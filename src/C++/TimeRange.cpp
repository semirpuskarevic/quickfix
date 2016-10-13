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
#include "stdafx.h"
#else
#include "config.h"
#endif

#include "TimeRange.h"
#include "Utility.h"

#include <iostream>

namespace FIX
{
  TimeRange::TimeRange( const UtcTimeOnly& startTime,
                        const UtcTimeOnly& endTime,
                        int startDay,
                        int endDay )
  : m_startTime( startTime ), m_endTime( endTime ),
    m_startDay( startDay ), m_endDay( endDay ),
    m_useLocalTime( false )
  {
    if( startDay > 0
        && endDay > 0
        && startDay == endDay
        && endTime > startTime )
    { m_endTime = m_startTime; }
  }

  TimeRange::TimeRange( const LocalTimeOnly& startTime,
                        const LocalTimeOnly& endTime,
                        int startDay,
                        int endDay )
  : m_startTime( startTime ), m_endTime( endTime ),
    m_startDay( startDay ), m_endDay( endDay ),
    m_useLocalTime( true )
  {
    if( startDay > 0
        && endDay > 0
        && startDay == endDay
        && endTime > startTime )
    { m_endTime = m_startTime; }
  }

  bool TimeRange::isInRange( const DateTime& start,
                             const DateTime& end,
                             const DateTime& time )
  {
    UtcTimeOnly timeOnly (time);

    if( start < end )
      return( timeOnly >= start && timeOnly <= end );
    else
      return( timeOnly >= start || timeOnly <= end );
  }


  bool TimeRange::isInRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
               const DateTime& time ) 
  {
    return isInRange( startTime, endTime, startDay, endDay, time, time.getWeekDay() );
  }

  bool TimeRange::isInSameRange( const DateTime& start,
                                 const DateTime& end,
                                 const DateTime& time1,
                                 const DateTime& time2 )
  {
    if( !isInRange( start, end, time1 ) ) return false;
    if( !isInRange( start, end, time2 ) ) return false;

    if( time1 == time2 ) return true;


    if( start < end || start == end )
    {
      UtcDate time1Date( time1 );
      UtcDate time2Date( time2 );

      return time1Date == time2Date;
    }
    else
    {
      int sessionLength = DateTime::SECONDS_PER_DAY - (start - end);

      if( time1 > time2 )
      {
        UtcTimeOnly time2TimeOnly = UtcTimeOnly(time2);

        long delta = time2TimeOnly - start;
        if( delta < 0 )
          delta = DateTime::SECONDS_PER_DAY - labs(delta);

        return (time1 - time2) < (sessionLength - delta);
      }
      else
      {
        return (time2 - time1) < sessionLength;
      }
    }
  }

  bool isDayInClosedRange(int startDay, int endDay, int day)
  {
      bool dayCheck = startDay <= day && day <= endDay;
      return startDay == endDay ? true : dayCheck;
  }

  bool isTimeInOpenIntradaySession( const DateTime& startTime,
                                const DateTime& endTime,
                                int startDay,
                                int endDay,
                                const UtcTimeOnly& timeOnly,
                                int day )
  {
      if(startDay == -1)
          return startTime < timeOnly && timeOnly < endTime;
      if(day != startDay)
          return false;
      else
          return startTime < timeOnly  && timeOnly < endTime;
  }

  bool isTimeInClosedIntradaySession( const DateTime& startTime,
                                const DateTime& endTime,
                                int startDay,
                                int endDay,
                                const UtcTimeOnly& timeOnly,
                                int day )
  {
      if(startDay == -1)
          return startTime <= timeOnly && timeOnly <= endTime;
      if(day != startDay)
          return false;
      else
          return startTime <= timeOnly  && timeOnly <= endTime;
  }
  // start and end times are not included
  bool isInOpenRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const UtcTimeOnly& timeOnly,
                             int day )
  {
      if( startTime < endTime)
      {
          if(startDay == endDay)
              return isTimeInOpenIntradaySession(startTime, endTime, startDay, endDay, timeOnly, day);
          else
          {
              bool timeCheck = true;
              timeCheck = day == startDay ? startTime < timeOnly : timeCheck;
              timeCheck = day == endDay ? timeOnly < endTime : timeCheck;
              return isDayInClosedRange(startDay, endDay, day) && timeCheck;
          }
      }
      else
      {
          // not intraday session
          bool invTimeCheck = false;
          if (day == startDay || day == endDay) invTimeCheck = endTime <= timeOnly && timeOnly <= startTime;
          return isDayInClosedRange(startDay, endDay, day) && !invTimeCheck;
      }
  }

  bool isInClosedRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const UtcTimeOnly& timeOnly,
                             int day )
  {
      if( startTime < endTime)
      {
          if(startDay == endDay)
              return isTimeInClosedIntradaySession(startTime, endTime, startDay, endDay, timeOnly, day);
          else
          {
              bool timeCheck = true;
              timeCheck = day == startDay ? startTime <= timeOnly : timeCheck;
              timeCheck = day == endDay ? timeOnly <= endTime : timeCheck;
              return isDayInClosedRange(startDay, endDay, day) && timeCheck;
          }
      }
      else
      {
          // not intraday session
          bool invTimeCheck = false;
          if (day == startDay || day == endDay) invTimeCheck = endTime < timeOnly && timeOnly < startTime;
          return isDayInClosedRange(startDay, endDay, day) && !invTimeCheck;
      }
  }

  bool TimeRange::isInRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time,
                             int day )
  {
      UtcTimeOnly timeOnly(time);

      if(startDay <= endDay)
          return isInClosedRange(startTime, endTime, startDay, endDay, time, day);
      else
          return !isInOpenRange(endTime, startTime, endDay, startDay, time, day);
  }


  bool areInSameSession( const DateTime& endTime,
                         int startDay,
                         int endDay,
                         const DateTime& time1,
                         const DateTime& time2 )
  {
      int absTime1 = time1.getJulianDate() - time1.getWeekDay();
      int absTime2 = time2.getJulianDate() - time2.getWeekDay();

      // same week
      if(absTime1 == absTime2)
          return true;

      // week crossing on Sunday
      if(startDay >= endDay && abs(absTime1 - absTime2) == 7)
      {
          DateTime largerTime(time1);
          if(time2 > time1)
              largerTime = time2;
          UtcTimeOnly midStartTime(0, 0, 0);
          return isInClosedRange(midStartTime, endTime, 1, endDay, largerTime, largerTime.getWeekDay());
      }
      return false;
  }

  bool TimeRange::isInSameRange( const DateTime& startTime,
                                 const DateTime& endTime,
                                 int startDay,
                                 int endDay,
                                 const DateTime& time1,
                                 const DateTime& time2 )
  {
      if(!isInRange(startTime, endTime, startDay, endDay, time1, time1.getWeekDay()))
        return false;
      if(!isInRange(startTime, endTime, startDay, endDay, time2, time2.getWeekDay()))
        return false;
      return areInSameSession(endTime, startDay, endDay, time1, time2);
  }
}
