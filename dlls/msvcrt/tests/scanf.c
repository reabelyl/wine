/*
 * Unit test suite for *scanf functions.
 *
 * Copyright 2002 Uwe Bonnes
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>

#include "wine/test.h"

static void test_sscanf( void )
{
    char buffer[100], buffer1[100];
    char format[20];
    int result, ret;
    float res1= -82.6267f, res2= 27.76f, res11, res12;
    char pname[]=" St. Petersburg, Florida\n";
    int hour=21,min=59,sec=20;
    int  number,number_so_far;

    
    /* check EOF */
    strcpy(buffer,"");
    ret = sscanf(buffer, "%d", &result);
    ok( ret == EOF,"sscanf returns %x instead of %x", ret, EOF );
		
    /* check %x */
    strcpy(buffer,"0x519");
    ok( sscanf(buffer, "%x", &result) == 1, "sscanf failed"  );
    ok( result == 0x519,"sscanf reads %x instead of %x", result, 0x519 );

    strcpy(buffer,"0x51a");
    ok( sscanf(buffer, "%x", &result) == 1, "sscanf failed" );
    ok( result == 0x51a ,"sscanf reads %x instead of %x", result, 0x51a );

    strcpy(buffer,"0x51g");
    ok( sscanf(buffer, "%x", &result) == 1, "sscanf failed" );
    ok( result == 0x51, "sscanf reads %x instead of %x", result, 0x51 );

    /* check % followed by any char */
    strcpy(buffer,"\"%12@");
    strcpy(format,"%\"%%%d%@");  /* work around gcc format check */
    ok( sscanf(buffer, format, &result) == 1, "sscanf failed" );
    ok( result == 12, "sscanf reads %x instead of %x", result, 12 );

    /*Check float */
    ret = sprintf(buffer,"%f %f",res1, res2);
    ret = sscanf(buffer,"%f%f",&res11, &res12);
    ok( (res11 == res1) && (res12 == res2), "Error reading floats");
    ret = sprintf(buffer," %s", pname);
    ret = sscanf(buffer,"%*c%[^\n]",buffer1);
    ok( ret == 1, "Error with format \"%s\"","%*c%[^\n]");
    ok( strncmp(pname,buffer1,strlen(buffer1)) == 0, "Error with \"%s\" \"%s\"",pname, buffer1);
    ret = sprintf(buffer,"%d:%d:%d",hour,min,sec);
    ret = sscanf(buffer,"%d%n",&number,&number_so_far);
    ok(ret == 1 , "problem with format arg \"%%d%%n\"");
    ok(number == hour,"Read wrong arg %d instead of %d",number, hour);
    ok(number_so_far == 2,"Read wrong arg for \"%%n\" %d instead of 2",number_so_far);
    ret = sscanf(buffer+2,"%*c%n",&number_so_far);
    ok(ret == 0 , "problem with format arg \"%%*c%%n\"");
    ok(number_so_far == 1,"Read wrong arg for \"%%n\" %d instead of 2",number_so_far);
}

static void test_sprintf( void )
{
    char buffer[100];
    const char *I64d = "%I64d";
    double pnumber=789456123;
    sprintf(buffer,"%+#23.15e",pnumber);
    todo_wine
      {
	ok(strstr(buffer,"e+008") != 0,"Sprintf different \"%s\"\n",buffer);
      }
    sprintf(buffer,I64d,((ULONGLONG)0xffffffff)*0xffffffff);
    todo_wine
      {
	ok(strlen(buffer) == 19,"Problem with long long \"%s\"\n",buffer);
      }
}

START_TEST(scanf)
{
    test_sscanf();
    test_sprintf();
}
