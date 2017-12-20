//
// deskhare
// Copyright (C) 2017 Richard Liebscher
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "calcparser.h"

#include <QChar>
#include <QString>
#include <QLoggingCategory>
#include <exception>
#include <stdexcept>
#include <vector>
#include <QDebug>
#include <cmath>
#include <iostream>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

// developed from
// https://github.com/boostorg/spirit/blob/develop/example/x3/calc/calc5.cpp
// Copyright (c) 2001-2014 Joel de Guzman

namespace x3 = boost::spirit::x3;

namespace Deskhare {
namespace Calc {

static Q_LOGGING_CATEGORY(logger, "deskhare.calc.parser")

namespace Ast {

////
// AST

struct Nil {};
struct UnaryOp;
struct Expression;

struct Operand : x3::variant<
  Nil, unsigned int, x3::forward_ast<UnaryOp>, x3::forward_ast<Expression>
>
{
  using base_type::base_type;
  using base_type::operator=;
};

struct UnaryOp
{
  char operator_;
  Operand operand_;
};

struct Operation
{
  char operator_;
  Operand operand_;
};

struct Expression
{
  Operand first;
  std::vector<Operation> rest;
};

// print function for debugging
inline std::ostream& operator<<(std::ostream& out, Nil)
{
  out << "nil";
  return out;
}

}}}

BOOST_FUSION_ADAPT_STRUCT(Deskhare::Calc::Ast::UnaryOp,
  operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(Deskhare::Calc::Ast::Operation,
  operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(Deskhare::Calc::Ast::Expression,
  first, rest
)

namespace Deskhare {
namespace Calc {
namespace Ast {

////
// Evaluator

struct Evaluator
{
  typedef int result_type;

  int operator()(Nil) const
  {
    Q_ASSERT(false);
    return 0;
  }

  int operator()(unsigned int n) const
  {
    return n;
  }

  int operator()(const Operation& x, int lhs) const
  {
    int rhs = boost::apply_visitor(*this, x.operand_);
    switch (x.operator_)
    {
    case '+':
      return lhs + rhs;
    case '-':
      return lhs - rhs;
    case '*':
      return lhs * rhs;
    case '/':
      return lhs / rhs;
    }
    Q_ASSERT(false);
    return 0;
  }

  int operator()(const UnaryOp& x) const
  {
    int rhs = boost::apply_visitor(*this, x.operand_);
    switch (x.operator_)
    {
    case '-':
      return -rhs;
    case '+':
      return +rhs;
    }
    Q_ASSERT(false);
    return 0;
  }

  int operator()(const Expression& x) const
  {
    int state = boost::apply_visitor(*this, x.first);
    for (const Operation& oper : x.rest)
    {
      state = (*this)(oper, state);
    }
    return state;
  }
};

} // namespace Ast


////
// Grammar

namespace Grammar {

using x3::uint_;
using x3::char_;

struct Expression_class;
struct Term_class;
struct Factor_class;

x3::rule<Expression_class, Ast::Expression> const Expression("Expression");
x3::rule<Term_class, Ast::Expression> const Term("Term");
x3::rule<Factor_class, Ast::Operand> const Factor("Factor");

auto const Expression_def =
  Term >> *((char_('+') > Term) | (char_('-') > Term));

auto const Term_def =
  Factor >> *((char_('*') > Factor) | (char_('/') > Factor));

auto const Factor_def =
  uint_
  | '(' > Expression > ')'
  | (char_('-') > Factor)
  | (char_('+') > Factor);

BOOST_SPIRIT_DEFINE(Expression, Term, Factor);

} // namespace Grammar

bool evalExpression(const QString& input, double& result)
{
  typedef std::wstring::const_iterator iterator_type;

  std::wstring str = input.toStdWString();
  iterator_type start = str.begin();
  iterator_type iter = start;
  iterator_type end = str.end();

  try
  {
    Ast::Expression program;

    bool r = phrase_parse(
      iter, end, Grammar::Expression, x3::ascii::space, program);
    if (r && iter == end)
    {
      result = Ast::Evaluator()(program);
      return true;
    }
    else
    {
      qCDebug(logger).noquote()
        << "could not parse everything:"
        << QString::fromWCharArray(&(*start), iter - start) << "!"
        << QString::fromWCharArray(&(*iter), end - iter);
    }
  }
  catch (const x3::expectation_failure<iterator_type>& exp)
  {
    std::wstring rest(exp.where(), end);
    qCDebug(logger).noquote()
      << "invalid expression: expected" << exp.which().c_str() <<
      "in" << QString::fromWCharArray(&(*start), exp.where() - start) << "!"
      << QString::fromWCharArray(&(*exp.where()), end - exp.where());
  }
  catch (const std::exception& e)
  {
    qCDebug(logger) << "could not parse:" << e.what();
  }
  catch (...)
  {
    qCWarning(logger) << "got unknown exception";
  }

  return false;
}

} // namespace Calc
} // namespace Deskhare
