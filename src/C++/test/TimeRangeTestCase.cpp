/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#include "stdafx.h"
#else
#include "config.h"
#endif

#include <UnitTest++.h>
#include <TimeRange.h>
#include <FieldConvertors.h>

using namespace FIX;

SUITE(TimeRangeTests)
{

TEST(isInRange)
{
  UtcTimeOnly start( 3, 0, 0 );
  UtcTimeOnly end( 18, 0, 0 );

  UtcTimeStamp now( 10, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 18, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 2, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 19, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 18, 0, 1, 10, 10, 2000 );
  CHECK( !TimeRange::isInRange( start, end, now ) );

  start = UtcTimeOnly( 18, 0, 0 );
  end = UtcTimeOnly( 3, 0, 0 );
  now = UtcTimeStamp( 18, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 3, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 4, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInRange( start, end, now ) );

  now = UtcTimeStamp( 17, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInRange( start, end, now ) );
}

TEST(isInRangeWithDay)
{
  UtcTimeOnly startTime( 3, 0, 0 );
  UtcTimeOnly endTime( 18, 0, 0 );
  int startDay = 2;
  int endDay = 5;

  UtcTimeStamp now( 2, 0, 0, 28, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 18, 0, 0, 27, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 3, 0, 0, 27, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 2, 59, 59, 26, 7, 2004 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 18, 0, 1, 29, 7, 2004 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));

  startDay = 5;
  endDay = 2;

  now = UtcTimeStamp( 2, 0, 0, 24, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 2, 0, 0, 28, 7, 2004 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 3, 0, 0, 22, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 18, 0, 0, 26, 7, 2004 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 2, 59, 59, 22, 7, 2004 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 18, 0, 1, 26, 7, 2004 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));

  startTime = UtcTimeOnly( 9, 1, 0 );
  endTime = UtcTimeOnly( 8, 59, 0 );
  startDay = 1;
  endDay = 1;

  now = UtcTimeStamp( 8, 59, 0, 3, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 8, 59, 1, 3, 12, 2006 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 9, 1, 0, 3, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 9, 0, 0, 3, 12, 2006 );
  CHECK(!TimeRange::isInRange(startTime, endTime, startDay, endDay, now));

  now = UtcTimeStamp( 8, 59, 0, 4, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 8, 59, 1, 4, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 9, 1, 0, 4, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
  now = UtcTimeStamp( 9, 0, 0, 4, 12, 2006 );
  CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now));
}

TEST(isInSameRange)
{
  // start time is less than end time
  UtcTimeOnly start( 3, 0, 0 );
  UtcTimeOnly end( 18, 0, 0 );

  // same time
  UtcTimeStamp time1( 10, 0, 0, 10, 10, 2000 );
  UtcTimeStamp time2( 10, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // time 2 in same session but greater
  time1 = UtcTimeStamp( 10, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 11, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // time 2 in same session but less
  time1 = UtcTimeStamp( 11, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 10, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // time 1 not in session
  time1 = UtcTimeStamp( 19, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 10, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( !TimeRange::isInSameRange( start, end, time2, time1 ) );

  // time 2 not in session
  time1 = UtcTimeStamp( 10, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 2, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( !TimeRange::isInSameRange( start, end, time2, time1 ) );

  // start time is greater than end time
  start = UtcTimeOnly( 18, 0, 0 );
  end = UtcTimeOnly( 3, 0, 0 );

  // same session same day
  time1 = UtcTimeStamp( 19, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 20, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // same session time 2 is in next day
  time1 = UtcTimeStamp( 19, 0, 0, 10, 10, 2000 );
  time2 = UtcTimeStamp( 2, 0, 0, 11, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // same session time 1 is in next day
  time1 = UtcTimeStamp( 2, 0, 0, 11, 10, 2000 );
  time2 = UtcTimeStamp( 19, 0, 0, 10, 10, 2000 );
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( TimeRange::isInSameRange( start, end, time2, time1 ) );

  // time 1 is 25 hours greater than time 2
  time1 = UtcTimeStamp( 21, 0, 0, 11, 10, 2000 );
  time2 = UtcTimeStamp( 20, 0, 0, 10, 10, 2000 );
  CHECK( !TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( !TimeRange::isInSameRange( start, end, time2, time1 ) );

  // start time is greater than end time
  start = UtcTimeOnly( 6, 0, 0 );
  end = UtcTimeOnly( 6, 0, 0 );
  time1 = UtcTimeStamp( 19, 10, 0, 1, 13, 2004 );
  time2 = UtcTimeStamp( 19, 06, 0, 1, 14, 2004 );
  CHECK( !TimeRange::isInSameRange( start, end, time1, time2 ) );
  CHECK( !TimeRange::isInSameRange( start, end, time2, time1 ) );

  start= UtcTimeOnly(0, 10, 0);
  end= UtcTimeOnly(0, 5, 0);
  time1 = UtcTimeStamp(0, 10, 1, 2, 10, 2016);
  time2 = UtcTimeStamp(0, 4, 59, 3, 10, 2016);
  CHECK( TimeRange::isInSameRange( start, end, time1, time2 ) );
}

TEST(isInSameRangeWithDay)
{
  UtcTimeOnly startTime( 3, 0, 0 );
  UtcTimeOnly endTime( 18, 0, 0 );
  int startDay = 2;
  int endDay = 5;

  UtcTimeStamp time1( 3, 0, 0, 27, 7, 2004 );
  UtcTimeStamp time2( 3, 0, 0, 25, 7, 2004 );
  CHECK( !TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 3, 0, 0, 31, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 27, 7, 2004 );
  CHECK( !TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 3, 0, 0, 27, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 27, 7, 2004 );
  CHECK( TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 10, 0, 0, 26, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 27, 7, 2004 );
  CHECK( TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 10, 0, 0, 27, 7, 2004 );
  time2 = UtcTimeStamp( 2, 0, 0, 29, 7, 2004 );
  CHECK( TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 10, 0, 0, 27, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 20, 7, 2004 );
  CHECK( !TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 2, 0, 0, 27, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 20, 7, 2004 );
  CHECK( !TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  time1 = UtcTimeStamp( 2, 0, 0, 26, 7, 2004 );
  time2 = UtcTimeStamp( 3, 0, 0, 19, 7, 2004 );
  CHECK( !TimeRange::isInSameRange
    ( startTime, endTime, startDay, endDay, time1, time2 ) );

  // Reset start/end time so that they fall within an hour of midnight
  startTime = UtcTimeOnly(0, 5, 0);
  endTime = UtcTimeOnly(23, 45, 0);

  // Make it a week-long session
  startDay = 1;
  endDay = 7;

  // Check that ST-->DST (Sunday is missing one hour) is handled
  time1 = UtcTimeStamp(0, 0, 0, 4, 4, 2006);
  time2 = UtcTimeStamp(1, 0, 0, 3, 4, 2006);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  // Check that DST-->ST (Sunday has an extra hour) is handled
  time1 = UtcTimeStamp(0, 0, 0, 30, 10, 2006);
  time2 = UtcTimeStamp(1, 0, 0, 31, 10, 2006);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  // Check that everything works across a year boundary
  time1 = UtcTimeStamp(10, 10, 10, 31, 12, 2006);
  time2 = UtcTimeStamp(10, 10, 10, 1, 1, 2007);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  // Check that "missing" start and end days are handled as isSameSession without days
  startDay = -1;
  endDay = -1;
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  // Session days are the same
  startDay = 1;
  endDay = 1;
  startTime = UtcTimeOnly(9, 1, 0);
  endTime = UtcTimeOnly(8, 59, 0);
  time1 = UtcTimeStamp(9, 1, 0, 3, 12, 2006);
  time2 = UtcTimeStamp(9, 1, 0, 3, 12, 2006);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );
  time2 = UtcTimeStamp(9, 1, 0, 10, 12, 2006);
  CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );
  time2 = UtcTimeStamp(9, 1, 0, 4, 12, 2006);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  time2 = UtcTimeStamp(9, 5, 0, 10, 12, 2006);
  CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2) );

  // Tests crossing Wednesday to Thursday
  startTime = UtcTimeOnly(0, 10, 0);
  endTime = UtcTimeOnly(0, 0, 0);
  time1 = UtcTimeStamp(0, 10, 1, 2, 10, 2016);

  time2 = UtcTimeStamp(0, 0, 1, 5, 10, 2016);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  time2 = UtcTimeStamp(0, 0, 1, 6, 10, 2016);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  time2 = UtcTimeStamp(0, 0, 1, 7, 10, 2016);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  time2 = UtcTimeStamp(22, 0, 1, 8, 10, 2016);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  endTime = UtcTimeOnly(0, 5, 0);
  time2 = UtcTimeStamp(0, 4, 59, 8, 10, 2016);
  CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
  
  time2 = UtcTimeStamp(0, 4, 59, 9, 10, 2016);
  CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  time2 = UtcTimeStamp(0, 5, 1, 9, 10, 2016);
  CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );

  // 
  startDay = 4;
  endDay = 4;
  startTime = UtcTimeOnly(0, 10, 0);
  endTime = UtcTimeOnly(0, 0, 0);

  time1 = UtcTimeStamp(0, 10, 1, 5, 10, 2016);
  time2 = UtcTimeStamp(0, 0, 1, 11, 10, 2016);
  CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
}

TEST(isTimePointInDayTimeRange)
{
    // NOTE:
    // time1 == startTime is always in range
    // time1 == endTime is always in range
    int startDay = 2;
    int endDay = 5;

    UtcTimeOnly startTime( 3, 0, 0 );
    UtcTimeOnly endTime( 18, 0, 0 );
    UtcTimeStamp time1 = UtcTimeStamp(2, 0, 0, 10, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(3, 0, 1, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    // time1 == startTime (boundary in range)
    time1 = UtcTimeStamp(3, 0, 0, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp( 2, 0, 0, 13, 10, 2016 );
    CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay()));

    // time1 == endTime (boundary in range)
    time1 = UtcTimeStamp(18, 0, 0, 13, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(19, 0, 0, 13, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp( 2, 0, 0, 12, 10, 2016 );
    CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay()));

    // same day
    startDay = 2;
    endDay = 2;

    //startTime < endTime
    time1 = UtcTimeStamp(2, 0, 0, 10, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(19, 0, 0, 10, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(15, 0, 0, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(15, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    startTime = UtcTimeOnly(8, 0, 0);
    endTime = UtcTimeOnly(16, 0, 0);
    time1 = UtcTimeStamp(15, 59, 59, 11, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    //startTime > endTime (week long session)
    startTime = UtcTimeOnly( 18, 0, 0 );
    endTime = UtcTimeOnly( 3, 0, 0 );

    // time1 == endTime (boundary in range)
    time1 = UtcTimeStamp(3, 0, 0, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(3, 0, 1, 10, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    
    // time1 == startTime (boundary in range)
    time1 = UtcTimeStamp(18, 0, 0, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(19, 0, 1, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(2, 0, 0, 10, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(2, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    
    // time1 == endTime (boundary in range)
    time1 = UtcTimeStamp(3, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(3, 0, 1, 17, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    //startDay < endDay (week-crossing)
    startDay = 5;
    endDay = 2;
    
    startTime = UtcTimeOnly( 3, 0, 0 );
    endTime = UtcTimeOnly( 18, 0, 0 );

    time1 = UtcTimeStamp(2, 59, 59, 13, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(18, 0, 1, 17, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    time1 = UtcTimeStamp(3, 0, 1, 13, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    
    time1 = UtcTimeStamp(3, 0, 1, 20, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    // time in range, but day out of range
    time1 = UtcTimeStamp(3, 0, 1, 12, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    // time in range, but day out of range
    time1 = UtcTimeStamp(3, 0, 1, 18, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    
    // time1 == startTime (boundary in range)
    time1 = UtcTimeStamp(3, 0, 0, 13, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    
    // time1 == endTime (boundary in range)
    time1 = UtcTimeStamp(18, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );


    startDay = 1;
    endDay = 1;
    startTime = UtcTimeOnly( 0, 10, 0 );
    endTime = UtcTimeOnly( 0, 0, 0 );
    time1 = UtcTimeStamp(5, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(0, 5, 0, 16, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    startDay = 7;
    endDay = 7;
    startTime = UtcTimeOnly( 0, 10, 0 );
    endTime = UtcTimeOnly( 0, 0, 0 );
    time1 = UtcTimeStamp(5, 0, 0, 17, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(0, 5, 0, 15, 10, 2016);
    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    time1 = UtcTimeStamp(23, 59, 59, 14, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    // time1 == endTime (boundary in range)
    time1 = UtcTimeStamp(0, 0, 0, 15, 10, 2016);
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );

    startTime = UtcTimeOnly( 3, 0, 0 );
    endTime = UtcTimeOnly( 18, 0, 0 );
    startDay = 2;
    endDay = 5;

    UtcTimeStamp now( 2, 0, 0, 28, 7, 2004 );
    CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now, now.getWeekDay()));

    startDay = 5;
    endDay = 2;

    now = UtcTimeStamp( 2, 0, 0, 8, 10, 2016 );
    CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now, now.getWeekDay()));

    now = UtcTimeStamp( 2, 0, 0, 7, 10, 2016 );
    CHECK(TimeRange::isInRange(startTime, endTime, startDay, endDay, now, now.getWeekDay()));
}


TEST(isInSameRangeOurFix)
{
    // startDay < endDay
    int startDay = 2;
    int endDay = 5;

    UtcTimeOnly startTime( 3, 0, 0 );
    UtcTimeOnly endTime( 18, 0, 0 );
    UtcTimeStamp time1 = UtcTimeStamp(3, 0, 1, 10, 10, 2016);
    UtcTimeStamp time2 = UtcTimeStamp(17, 59, 59, 13, 10, 2016);
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    time1 = UtcTimeStamp(2, 0, 0, 10, 10, 2016);
    time2 = UtcTimeStamp(15, 0, 0, 10, 10, 2016);
    // time 1 is out of range
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    startDay = 1;
    endDay = 7;
    startTime = UtcTimeOnly( 3, 0, 0 );
    endTime = UtcTimeOnly( 2, 50, 0 );
    time1 = UtcTimeStamp(3, 0, 1, 9, 10, 2016);
    time2 = UtcTimeStamp(2, 49, 59, 15, 10, 2016);
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    // startDay == endDay - intraday session
    startDay = 2;
    endDay = 2;
    startTime = UtcTimeOnly(8, 0, 0);
    endTime = UtcTimeOnly(16, 0, 0);
    time1 = UtcTimeStamp(8, 0, 1, 10, 10, 2016);
    time2 = UtcTimeStamp(15, 59, 59, 10, 10, 2016);
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    time2 = UtcTimeStamp(18, 0, 0, 10, 10, 2016);
    // time 2 is out of range
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    time1 = UtcTimeStamp(7, 0, 1, 10, 10, 2016);
    time2 = UtcTimeStamp(15, 59, 59, 10, 10, 2016);
    // time 1 is out of range
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );

    CHECK( !TimeRange::isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay())  );
    CHECK( TimeRange::isInRange(startTime, endTime, startDay, endDay, time2, time2.getWeekDay())  );

    startDay = 2;
    endDay = 2;
    startTime = UtcTimeOnly(8, 0, 0);
    endTime = UtcTimeOnly(16, 0, 0);
    time1 = UtcTimeStamp(8, 0, 1, 10, 10, 2016);
    time2 = UtcTimeStamp(15, 59, 59, 11, 10, 2016);
    // time 2 is out of range (date is out of range)
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time1, time2)  );
    CHECK( !TimeRange::isInSameRange(startTime, endTime, startDay, endDay, time2, time1)  );
}
}
