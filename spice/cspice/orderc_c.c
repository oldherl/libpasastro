/*

-Procedure orderc_c ( Order of a character array )

-Abstract
 
   Determine the order of elements in an array of character strings. 
 
-Disclaimer

   THIS SOFTWARE AND ANY RELATED MATERIALS WERE CREATED BY THE
   CALIFORNIA INSTITUTE OF TECHNOLOGY (CALTECH) UNDER A U.S.
   GOVERNMENT CONTRACT WITH THE NATIONAL AERONAUTICS AND SPACE
   ADMINISTRATION (NASA). THE SOFTWARE IS TECHNOLOGY AND SOFTWARE
   PUBLICLY AVAILABLE UNDER U.S. EXPORT LAWS AND IS PROVIDED "AS-IS"
   TO THE RECIPIENT WITHOUT WARRANTY OF ANY KIND, INCLUDING ANY
   WARRANTIES OF PERFORMANCE OR MERCHANTABILITY OR FITNESS FOR A
   PARTICULAR USE OR PURPOSE (AS SET FORTH IN UNITED STATES UCC
   SECTIONS 2312-2313) OR FOR ANY PURPOSE WHATSOEVER, FOR THE
   SOFTWARE AND RELATED MATERIALS, HOWEVER USED.

   IN NO EVENT SHALL CALTECH, ITS JET PROPULSION LABORATORY, OR NASA
   BE LIABLE FOR ANY DAMAGES AND/OR COSTS, INCLUDING, BUT NOT
   LIMITED TO, INCIDENTAL OR CONSEQUENTIAL DAMAGES OF ANY KIND,
   INCLUDING ECONOMIC DAMAGE OR INJURY TO PROPERTY AND LOST PROFITS,
   REGARDLESS OF WHETHER CALTECH, JPL, OR NASA BE ADVISED, HAVE
   REASON TO KNOW, OR, IN FACT, SHALL KNOW OF THE POSSIBILITY.

   RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF
   THE SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY
   CALTECH AND NASA FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE
   ACTIONS OF RECIPIENT IN THE USE OF THE SOFTWARE.

-Required_Reading
 
   None. 
 
-Keywords
 
   ARRAY,  SORT 
 
*/

   #include "SpiceUsr.h"
   #include "SpiceZfc.h"
   #include "SpiceZst.h"
   #include "SpiceZmc.h"
   #undef    orderc_c


   void orderc_c ( SpiceInt      lenvals,
                   const void  * array,
                   SpiceInt      ndim,
                   SpiceInt    * iorder  ) 

/*

-Brief_I/O
 
   VARIABLE  I/O  DESCRIPTION 
   --------  ---  -------------------------------------------------- 
   lenvals    I   String length.
   array      I   Input array. 
   ndim       I   Dimension of array. 
   iorder     O   Order vector for array. 
 
-Detailed_Input
 
   lenvals     is the declared length of the strings in the input
               string array, including null terminators.  The input   
               array should be declared with dimension 

                  [ndim][lenvals]

   array       is the input array. 
 
   ndim        is the number of elements in the input array. 
 
-Detailed_Output
 
   iorder      is the order vector for the input array. 
               iorder[0] is the index of the smallest element 
               of array; iorder[1] is the index of the next 
               smallest; and so on. Strings are ordered according 
               to the ASCII collating sequence.  Trailing white space
               is ignored when comparing strings.

               The elements of iorder range from zero to ndim-1.
 
-Parameters
 
   None. 
 
-Exceptions
 
   1) If the input string pointer is null, the error
      SPICE(NULLPOINTER) will be signaled.
 
   2) If the input array string's length is less than 2, the error
      SPICE(STRINGTOOSHORT) will be signaled.

   3) If ndim < 1, this routine returns immediately.  This case is not 
      considered an error.

-Files
 
   None. 
 
-Particulars
 
   orderc_c finds the index of the smallest element of the input 
   array. This becomes the first element of the order vector. 
   The process is repeated for the rest of the elements. 
 
   The order vector returned by orderc_c may be used by any of 
   the reord* routines to sort sets of related arrays, as shown 
   in the example below. 
 
-Examples
 
   In the following example, the order and reord routines are 
   used to sort four related arrays (containing the names, 
   masses, integer ID codes, and visual magnitudes for a group   
   of satellites). This is representative of the typical use of 
   these routines. 


       #include "SpiceUsr.h"
           .
           .
           .
       /.
       Sort the object arrays by name. 
       ./ 
       
       orderc_c ( namlen, names, n,  iorder ); 
 
       reordc_c ( iorder, n, namlen, names  );
       reordd_c ( iorder, n,         masses ); 
       reordi_c ( iorder, n,         codes  ); 
       reordd_c ( iorder, n,         vmags  );    


-Restrictions
  
   1)  String comparisons performed by this routine are Fortran-style:
       trailing blanks in the input array or key value are ignored.
       This gives consistent behavior with CSPICE code generated by
       the f2c translator, as well as with the Fortran SPICE Toolkit.
      
       Note that this behavior is not identical to that of the ANSI
       C library functions strcmp and strncmp.
  
-Author_and_Institution
 
   N.J. Bachman    (JPL)
   I.M. Underwood  (JPL) 
 
-Literature_References
 
   None. 
 
-Version
 
   -CSPICE Version 1.0.0, 18-JUL-2002 (NJB) (IMU)

-Index_Entries
 
   order of a character array 
 
-&
*/

{ /* Begin orderc_c */


   /*
   Local variables
   */
   SpiceChar             * fCvalsArr;
   
   SpiceInt                fCvalsLen;
   SpiceInt                i;


   /*
   Participate in error tracing.
   */
   chkin_c ( "orderc_c" );

   /*
   Return immediately if the array dimension is non-positive. 
   */
   if ( ndim < 1 ) 
   {
      chkout_c ( "orderc_c" );
      return;
   }

   /*
   Make sure the input pointer for the string array is non-null 
   and that the length lenvals is sufficient.  
   */
   CHKOSTR ( CHK_STANDARD, "orderc_c", array, lenvals );
   

   /*
   Create a Fortran-style string array.
   */
   C2F_MapStrArr ( "orderc_c", 
                   ndim, lenvals, array, &fCvalsLen, &fCvalsArr );

   if ( failed_c() )
   {
      chkout_c ( "orderc_c" );
      return;
   }


   /*
   Call the f2c'd routine.
   */
   orderc_ (  ( char       * ) fCvalsArr,
              ( integer    * ) &ndim,
              ( integer    * ) iorder,
              ( ftnlen       ) fCvalsLen     );

   /*
   Free the dynamically allocated array.
   */
   free ( fCvalsArr );


   /*
   Map the order vector elements to the range 0 : ndim-1.
   */
   for ( i = 0;  i < ndim;  i++ )
   {
      --iorder[i];
   } 


   chkout_c ( "orderc_c" );

} /* End orderc_c */
