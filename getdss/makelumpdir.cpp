// same as makelump but work with directory input instead of CD letter 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define XFER_BUFF_SIZE 16384

int make_real_sky_plate_lump( const char *cd_drive_letter,
                                           const char *plate_name)
{
   char filename[100], *xfer_buff;
   FILE *outfile;
   int i, err_code = 0;
   uint32_t *index;

   index = (uint32_t *)calloc( 28 * 28 + 1, sizeof( uint32_t));
   xfer_buff = (char *)malloc( XFER_BUFF_SIZE);
   if( !index || !xfer_buff)
      {
      if( index)
         free( index);
      if( xfer_buff)
         free( xfer_buff);
      return( -1);
      }
   sprintf( filename, "%s.rsl", plate_name);
   outfile = fopen( filename, "wb");
   if( !outfile)
      {
      free( index);
      free( xfer_buff);
      return( -2);
      }

            /* First tile will start right after the index: */
   index[0] = (28 * 28 + 1) * sizeof( uint32_t);
   fwrite( index, (size_t)index[0], 1, outfile);
   for( i = 0; !err_code && i < 28 * 28; i++)
      {
      const char *letters = "0123456789abcdefghijklmnopqrstuvwxyz";
      FILE *ifile;

      sprintf( filename, "%s/%s/%s.%c%c", cd_drive_letter,
                      plate_name, plate_name,
                      letters[i / 28], letters[i % 28]);
           
      ifile = fopen( filename, "rb");
      if( !ifile)
         err_code = -3;
      else
         {
         uint32_t bytes_transferred = 0;
         unsigned read_size;

         while( !err_code &&
                  (read_size = fread( xfer_buff, 1, XFER_BUFF_SIZE, ifile)))
            {
            if( fwrite( xfer_buff, 1, read_size, outfile) != read_size)
               err_code = -4;
            bytes_transferred += read_size;
            }
         fclose( ifile);
         index[i + 1] = index[i] + bytes_transferred;
         }
      }
   free( xfer_buff);
   if( !err_code)      /* We now have a 'correct' index;  write it out */
      {
      fseek( outfile, 0L, SEEK_SET);
      fwrite( index, (size_t)index[0], 1, outfile);
      }
   free( index);
   fclose( outfile);
   return( err_code);
}

/*

   To make RealSky Lump files (.RSL) for every plate on a CD,  we
use the rather primitive strategy of calling make_real_sky_plate_lump( )
for all possible plates on every CD.  Most of them won't be on the
current CD,  of course.   */

int lump_entire_cd( const char *cd_drive_letter)
{
   int pass, i, n_plates = 0, plates_done = 0;
   time_t t0 = 0;

   for( pass = 0; pass < 2; pass++)
      for( i = 0; i < 765 + 896 + 881 + 7; i++)
         {
         char plate_name[20], filename[100];
         FILE *test_open;

         if( !i && pass == 1)          /* start of second pass */
            t0 = time( NULL);
         if( i < 765)
            sprintf( plate_name, "xe%03d", (i ? i : 1001));
         else if( i < 765 + 896)
            sprintf( plate_name, "s%03d", i - 765);
         else if( i < 765 + 896 + 881)
            sprintf( plate_name, "xv%03d", i - 765 - 896);
         else
            sprintf( plate_name, "xx%03d", i - 765 - 896 - 881);

         sprintf( filename, "%s/%s/%s.00", cd_drive_letter,
                      plate_name, plate_name);
         test_open = fopen( filename, "rb");

         if( test_open)
            {
            fclose( test_open);
            if( pass)
               {
               long dt = time( NULL) - t0;

               plates_done++;
               printf( "Lumping plate %d of %d; %ld seconds elapsed",
                          plates_done, n_plates, dt);
               if( plates_done > 1)
                  printf( ", %ld remain",
                               dt * (long)( n_plates - plates_done + 1) /
                                    (long)( plates_done - 1));
               printf( "\r");
               make_real_sky_plate_lump( cd_drive_letter, plate_name);
               }
            else
               n_plates++;
            }
         }
   return( n_plates);
}

int main( const int argc, const char **argv)
{
   int err_code = -99;

   if( argc == 3)             /* just lumping _one_ plate */
      err_code = make_real_sky_plate_lump( argv[1], argv[2]);
   else if( argc == 2)        /* lumping all plates on one CD */
      err_code = lump_entire_cd( argv[1]);
   else
      {
      printf( "makelump requires the CD-ROM drive letter as a command\n");
      printf( "line argument.  Optionally,  you can specify a single\n");
      printf( "plate to be extracted as well (instead of having all\n");
      printf( "plates on the CD processed).\n");
      }
   printf( "Err code: %d\n", err_code);
   return( 0);
}
