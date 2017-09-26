#include "thumbnails.h"

#include <QtTest>
#include <QDebug>

#include <xdgthumbnails.h>



void XdgThumbnailsTest::testQUrlFileSchema()
{
  QCOMPARE(
    QUrl::fromLocalFile("/proc/cpuinfo").toEncoded(),
    QByteArray("file:///proc/cpuinfo")
  );

  QCOMPARE(
    QUrl::fromLocalFile(
      QString::fromUtf16(u"/tmp/!\"#$%&'()*+,-.:<=>?@\\^_`{|}~.m4v")
    ).toEncoded(),
    QByteArray(
      "file:///tmp/"
      "!%22%23$%25&'()*+,-.:%3C=%3E%3F@%5C%5E_%60%7B%7C%7D~.m4v"
    )
  );
}

void XdgThumbnailsTest::testThumbnailPath()
{
  QCOMPARE(
    XdgThumbnails::getThumbnailFile(
      QUrl::fromLocalFile("/tmp/abc.jpg")
    ),
    QString("/tmp/.cache/thumbnails/large/f20e67cd3388da9f2eb049520ecd1941.png")
  );

  // Normal size
  QCOMPARE(
    XdgThumbnails::getThumbnailFile(
      QUrl::fromLocalFile("/tmp/abc.jpg"),
      XdgThumbnail::Normal
    ),
    QString(
      "/tmp/.cache/thumbnails/normal/f20e67cd3388da9f2eb049520ecd1941.png")
  );

  // Space
  QCOMPARE(
    XdgThumbnails::getThumbnailFile(
      QUrl::fromLocalFile("/tmp/abc def.jpg")
    ),
    QString("/tmp/.cache/thumbnails/large/cf5969bacc7b97771ab7da815d8c5fe8.png")
  );

  // ASCII
  QCOMPARE(
    XdgThumbnails::getThumbnailFile(
      QUrl::fromLocalFile(
        QString::fromUtf16(u"/tmp/!\"#$%&'()*+,-.:;<=>?@[\\]^_`{|}~.m4v")
      )
    ),
    QString("/tmp/.cache/thumbnails/large/0eead8ed983fec20bcb1ceff085cbce4.png")
  );

  // Unicode
  QCOMPARE(
    XdgThumbnails::getThumbnailFile(
      QUrl::fromLocalFile(
        QString::fromUtf16(u"/tmp/⚡😀🙄.m4v")
      )
    ),
    QString("/tmp/.cache/thumbnails/large/c1a6865581c49637b9da0ee12b131622.png")
  );

  // empty or relative
  QCOMPARE(XdgThumbnails::getThumbnailFile(QUrl()), QString());
  QCOMPARE(XdgThumbnails::getThumbnailFile(QUrl("../nothing")), QString());

  // TODO: server urls
}


QTEST_MAIN(XdgThumbnailsTest)

