#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "sort.h"
#include "ascrobbler.h"

int main(int argc, char **argv) 
{
	int pc = 0;
	int prev_id = -1;
	int artist_id;
	int n = 0;
	int i = 0;
	int j = 0;
	  
	struct list *curr = NULL;
	struct list *artists = NULL;
	struct list *plays = NULL;
	struct list *output = NULL;
	struct artist **index = NULL;

	struct play *p = NULL;
	struct artist *a = NULL;
	struct artist *b = NULL;
	struct artist *a2 = NULL;
 
	if( argc != 4 ) 
		{
		printf("USAGE: recommend artists_file plays_file artist_id\n");
		exit(1);
		} 
			
	if ( argv [1] == NULL || argv [2] == NULL )
		{
		printf("Error - Invalid File!\n");
		exit(1);
		}
		
	artist_id = atoi(argv[3]);
	
	if ( argv[3] <= 0 )
		{
		printf("Error - Artist ID cannot be less than or equal to zero\n");
		exit(1);
		}
		
    fprintf(stderr, "Initialising data structures...\n");
    fprintf(stderr, " - reading artists\t\t\t\t\t\t\t(step 1/8)\n");
    artists = read_list(argv[1], parse_artist);

    fprintf(stderr, " - reading plays\t\t\t\t\t\t\t(step 2/8)\n");
    plays = read_list(argv[2], parse_play);
    
    fprintf(stderr, " - counting artists\t\t\t\t\t\t\t(step 3/8)\n");
    n = length(artists);

    fprintf(stderr, " - sorting plays\t\t\t\t\t\t\t(step 4/8)\n");
    plays = sort(plays, p_leq_uid_geq_count);
  
    fprintf(stderr, " - sorting artists\t\t\t\t\t\t\t(step 5/8)\n");
    artists = sort(artists, a_leq_id);

    fprintf(stderr, " - indexing artists\t\t\t\t\t\t\t(step 6/8)\n");
    index = create_index(artists, n);
   
    a = lookup_artist(index, n, artist_id); 
	if (a == NULL)
		{
		printf("Artist ID not found\n");		//error checking - tells user that the artist id input is invalid and causes the program to close
		exit(1);
		}
		
    fprintf(stderr, " - registering fans\t\t\t\t\t\t\t(step 7/8)\n");
    register_fans(index, n, plays);
  
	fprintf(stderr, " - calculating similarities for artist id %d\t\t\t(step 8/8)\n", artist_id);  
    output = sort_similarity(a, artists);
	
	fprintf(stderr, "\n");
    fprintf(stderr, "Done - 25 similar artists will be printed out below\n\n");
	
	print_n(26, output, print_artist);			//prints out 25 artists that are the most similar to the input artist
	
    free_list(artists, free);
    free_list(plays, free);
    free_index(index);
}