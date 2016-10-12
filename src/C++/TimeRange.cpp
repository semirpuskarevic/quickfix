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


  /* bool TimeRange::isInRange( const DateTime& startTime, */
  /*                            const DateTime& endTime, */
  /*                            int startDay, */
  /*                            int endDay, */
  /*                            const DateTime& time, */
  /*                            int day ) */
  /* { */
  /*   UtcTimeOnly timeOnly (time); */

  /*   if( startDay == endDay ) */
  /*   { */
  /*     if( day != startDay ) */
  /*       return true; */
  /*     return isInRange( startTime, endTime, time ); */
  /*   } */
  /*   else if( startDay < endDay ) */
  /*   { */
  /*     if( day < startDay || day > endDay ) */
  /*       return false; */
  /*     else if( day == startDay && timeOnly < startTime ) */
  /*       return false; */
  /*     else if( day == endDay && timeOnly > endTime ) */
  /*       return false; */
  /*   } */
  /*   else if( startDay > endDay ) */
  /*   { */
  /*     if( day < startDay && day > endDay ) */
  /*       return false; */
  /*     else if( day == startDay && timeOnly < startTime ) */
  /*       return false; */
  /*     else if( day == endDay && timeOnly > endTime ) */
  /*       return false; */
  /*   } */
  /*   return true; */
  /* } */

  bool TimeRange::isInRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
               const DateTime& time ) 
  {
    return isInRange( startTime, endTime, startDay, endDay, time, time.getWeekDay() );
  }

  void printDT(const DateTime& dt) {
      std::cout<<dt.getYear()<<' '<<dt.getDay()<<' '<<dt.getMonth()<<' '<<dt.getHour()<<
          ':'<<dt.getMinute()<<':'<<dt.getSecond()<<'.'<<dt.getMillisecond()<<std::endl;
  }

  bool TimeRange::isInSameRange( const DateTime& start,
                                 const DateTime& end,
                                 const DateTime& time1,
                                 const DateTime& time2 )
  {
    std::cout<<"4params - StartTime: ";
    printDT(start);
    std::cout<<"4params - EndTime: ";
    printDT(end);
    std::cout<<"4params - Time1: ";
    printDT(time1);
    std::cout<<"4params - Time2: ";
    printDT(time2);

    if( !isInRange( start, end, time1 ) ) 
    {
        std::cout << "Time1 not in range" << std::endl;
        return false;
    }
    if( !isInRange( start, end, time2 ) )
    {
        std::cout << "Time2 not in range" << std::endl;
        return false;
    }

    if( time1 == time2 ) 
    {
        std::cout << "Time1 = Time2" << std::endl;
        return true;
    }


    if( start < end || start == end )
    {
      UtcDate time1Date( time1 );
      UtcDate time2Date( time2 );
 
      std::cout << "start <= end" << std::endl;
      return time1Date == time2Date;
    }
    else
    {
      int sessionLength = DateTime::SECONDS_PER_DAY - (start - end);
      std::cout << "Session length: " << sessionLength << std::endl;

      if( time1 > time2 )
      {
        UtcTimeOnly time2TimeOnly = UtcTimeOnly(time2);

        long delta = time2TimeOnly - start;

        std::cout << "Delta: "<<delta << std::endl;
        if( delta < 0 )
          delta = DateTime::SECONDS_PER_DAY - labs(delta);
        std::cout << "Delta after correction: "<<delta << std::endl;

        std::cout << "Time diff: " <<time1 - time2<<" Session diff: " <<sessionLength - delta<< std::endl;
        return (time1 - time2) < (sessionLength - delta);
      }
      else
      {
        std::cout << "(time2 >= time1) Time diff: " <<time2 - time1<<" Session length: " <<sessionLength<< std::endl;
        return (time2 - time1) < sessionLength;
      }
    }
  }


  /* bool TimeRange::isInSameRange( const DateTime& startTime, */
  /*                                const DateTime& endTime, */
  /*                                int startDay, */
  /*                                int endDay, */
  /*                                const DateTime& time1, */
  /*                                const DateTime& time2 ) */
  /* { */
  /*   std::cout<<"StartTime: "; */
  /*   printDT(startTime); */
  /*   std::cout<<"EndTime: "; */
  /*   printDT(endTime); */

  /*   if( !isInRange( startTime, endTime, startDay, endDay, time1, time1.getWeekDay() ) ) */
  /*   { */
  /*       std::cout << "First isInRange" << std::endl; */
  /*       return false; */
  /*   } */

  /*   if( !isInRange( startTime, endTime, startDay, endDay, time2, time2.getWeekDay() ) ) */
  /*   { */
  /*       std::cout << "Second isInRange" << std::endl; */
  /*       return false; */
  /*   } */

  /*   std::cout<<"Time1: "; */
  /*   printDT(time1); */
  /*   std::cout<<"Time2: "; */
  /*   printDT(time2); */
  /*   int absoluteDay1 = time1.getJulianDate() - time1.getWeekDay(); */
  /*   int absoluteDay2 = time2.getJulianDate() - time2.getWeekDay(); */
  /*   std::cout<<"Abs day1: "<<absoluteDay1<<std::endl; */
  /*   std::cout<<"Abs day2: "<<absoluteDay2<<std::endl; */
  /*   if (absoluteDay1 == absoluteDay2) */
  /*       return true; */
  /*   if ((absoluteDay1 + 7) == absoluteDay2) */
  /*   { */
  /*       /1* long delta = labs(timer2 - time1); // number of seconds *1/ */
  /*       if(time1 == time2) */
  /*           return false; */

  /*       return true; */
  /*       /1* UtcTimeOnly nextWeekTime1(time1); *1/ */

  /*       /1* if(!isInSameRange(startTime, endTime, nextWeekTime1, UtcTimeOnly(time2))) *1/ */
  /*       /1* /2* if( !isInRange( startTime, endTime, startDay, endDay, nextWeekTime1, nextWeekTime1.getWeekDay() ) ) *2/ *1/ */
  /*       /1* { *1/ */
  /*       /1*     std::cout << "Third failed isInRange" << std::endl; *1/ */
  /*       /1*     return false; *1/ */
  /*       /1* } *1/ */
  /*       /1* if(labs(time1.getJulianDate() - time2.getJulianDate()) <=7) *1/ */
  /*       /1*     return true; *1/ */
  /*   } */
  /*   return false; */
  /* } */

  // start and end times are not included
  bool isInOpenRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time,
                             int day )
  {
      UtcTimeOnly timeOnly(time);

      bool dayCheck = startDay <= day && day <= endDay;
      if( startDay == endDay ) dayCheck = true;
      if( startTime < endTime)
      {
          if(startDay  == endDay && startDay == -1)
          {
              return startTime < timeOnly  && timeOnly < endTime;
          }
          else
          {

              if(startDay == endDay && day != startDay)
                  return false;

              bool timeCheck = true;
              timeCheck = day == startDay ? startTime < timeOnly : timeCheck;
              timeCheck = day == endDay ? timeOnly < endTime : timeCheck;
              if(startDay == endDay && startDay == day)
                  timeCheck =  startTime < timeOnly  && timeOnly < endTime;
              /* if (day == startDay || day == endDay) timeCheck =  startTime < timeOnly  && timeOnly < endTime; */

              return dayCheck && timeCheck;
          }
      }
      else
      {
          bool invTimeCheck = false;
          if (day == startDay || day == endDay) invTimeCheck = endTime <= timeOnly && timeOnly <= startTime;
          return dayCheck && !invTimeCheck;
      }
  }

  bool isInClosedRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time,
                             int day )
  {
      UtcTimeOnly timeOnly(time);

      bool dayCheck = startDay <= day && day <= endDay;
      if( startDay == endDay ) dayCheck = true;
      if( startTime < endTime)
      {
          if(startDay  == endDay && startDay == -1)
          {
              return startTime <= timeOnly  && timeOnly <= endTime;
          }
          else
          {
              if(startDay == endDay && day != startDay)
                  return false;

              bool timeCheck = true;
              timeCheck = day == startDay ? startTime <= timeOnly : timeCheck;
              timeCheck = day == endDay ? timeOnly <=endTime : timeCheck;
              if(startDay == endDay && startDay == day)
                  timeCheck =  startTime <= timeOnly  && timeOnly <= endTime;

              /* if (day == startDay || day == endDay) timeCheck =  startTime <= timeOnly  && timeOnly <= endTime; */

              return dayCheck && timeCheck;
          }
      }
      else
      {
          bool invTimeCheck = false;
          if (day == startDay || day == endDay) invTimeCheck = endTime < timeOnly && timeOnly < startTime;
          return dayCheck && !invTimeCheck;
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
      {
          return isInClosedRange(startTime, endTime, startDay, endDay, time, day);
      }
      else
      {
          return !isInOpenRange(endTime, startTime, endDay, startDay, time, day);
      }
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
      int absTime1 = time1.getJulianDate() - time1.getWeekDay();
      int absTime2 = time2.getJulianDate() - time2.getWeekDay();

      if(absTime1 == absTime2)
          return true;
      if(startDay >= endDay && abs(absTime1 - absTime2) == 7)
      {
          DateTime t(time1);
          if(time2 > time1)
              t = time2;
          UtcTimeOnly midStartTime(0, 0, 0);
          return isInClosedRange(midStartTime, endTime, 1, endDay, t, t.getWeekDay());
      }
      return false;
  }
}
