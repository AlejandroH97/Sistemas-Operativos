/*
CPP program to demonstrate optimal page
replacement algorithm.
OPTIMAL: Tomado y entendido de GeeksforGeeks: https://www.geeksforgeeks.org/program-optimal-page-replacement-algorithm/
LRU: Tomado y entendido de GeeksforGeeks: https://www.geeksforgeeks.org/program-page-replacement-algorithms-set-1-lru/
FIFO: Tomado y entendido de geeksforgeeks: https://www.geeksforgeeks.org/program-page-replacement-algorithms-set-2-fifo/
Durante el transcurso del ejercicio se presentarios varios problemas como:
- Retornar un array de una funcion int
- Llenar el arreglo de manera aleatoria
- el arreglo de la funcion generateReferenceString no pasaba al arreglo pages[]
- punteros y demas en las funciones

Links de ayuda:
- http://www.cplusplus.com/forum/beginner/6644/
- http://www.cplusplus.com/reference/cstdlib/rand/
- https://stackoverflow.com/questions/3473438/return-array-in-a-function7
- https://foro.elhacker.net/programacion_cc/c_funcion_que_retorne_arrays-t84925.0.html
*/

#include <bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>		/* printf, scanf, puts, NULL */
#include <stdlib.h>		/* srand, rand */
#include <time.h>
#include <iostream>

using namespace std;

//FIFO

// Function to find page faults using FIFO
int
fifoPage (int pages[], int n, int capacity)
{
  // To represent set of current pages. We use
  // an unordered_set so that we quickly check
  // if a page is present in set or not
  unordered_set < int >s;

  // To store the pages in FIFO manner
  queue < int >indexes;

  // Start from initial page
  int page_faults = 0;
  for (int i = 0; i < n; i++)
    {
      // Check if the set can hold more pages
      if (s.size () < capacity)
	{
	  // Insert it into set if not present
	  // already which represents page fault
	  if (s.find (pages[i]) == s.end ())
	    {
	      s.insert (pages[i]);

	      // increment page fault
	      page_faults++;

	      // Push the current page into the queue
	      indexes.push (pages[i]);
	    }
	}

      // If the set is full then need to perform FIFO
      // i.e. remove the first page of the queue from
      // set and queue both and insert the current page
      else
	{
	  // Check if current page is not already
	  // present in the set
	  if (s.find (pages[i]) == s.end ())
	    {
	      //Pop the first page from the queue
	      int val = indexes.front ();

	      indexes.pop ();

	      // Remove the indexes page
	      s.erase (val);

	      // insert the current page
	      s.insert (pages[i]);

	      // push the current page into
	      // the queue
	      indexes.push (pages[i]);

	      // Increment page faults
	      page_faults++;
	    }
	}
    }

  return page_faults;
}


//LRU

int
lruPage (int pages[], int n, int capacity)
{
  // To represent set of current pages. We use
  // an unordered_set so that we quickly check
  // if a page is present in set or not
  unordered_set < int >s;

  // To store least recently used indexes
  // of pages.
  unordered_map < int, int >indexes;

  // Start from initial page
  int page_faults = 0;
  for (int i = 0; i < n; i++)
    {
      // Check if the set can hold more pages
      if (s.size () < capacity)
	{
	  // Insert it into set if not present
	  // already which represents page fault
	  if (s.find (pages[i]) == s.end ())
	    {
	      s.insert (pages[i]);

	      // increment page fault
	      page_faults++;
	    }

	  // Store the recently used index of
	  // each page
	  indexes[pages[i]] = i;
	}

      // If the set is full then need to perform lru
      // i.e. remove the least recently used page
      // and insert the current page
      else
	{
	  // Check if current page is not already
	  // present in the set
	  if (s.find (pages[i]) == s.end ())
	    {
	      // Find the least recently used pages
	      // that is present in the set
	      int lru = INT_MAX, val;
	      for (auto it = s.begin (); it != s.end (); it++)
		{
		  if (indexes[*it] < lru)
		    {
		      lru = indexes[*it];
		      val = *it;
		    }
		}

	      // Remove the indexes page
	      s.erase (val);

	      // insert the current page
	      s.insert (pages[i]);

	      // Increment page faults
	      page_faults++;
	    }

	  // Update the current page index
	  indexes[pages[i]] = i;
	}
    }

  return page_faults;
}




//OPTIMAL

// Function to check whether a page exists
// in a frame or not
bool
search (int key, vector < int >&fr)
{
  for (int i = 0; i < fr.size (); i++)
    if (fr[i] == key)
      return true;
  return false;
}

// Function to find the frame that will not be used
// recently in future after given index in pg[0..pn-1]
int
predict (int pg[], vector < int >&fr, int pn, int index)
{
  // Store the index of pages which are going
  // to be used recently in future
  int res = -1, farthest = index;
  for (int i = 0; i < fr.size (); i++)
    {
      int j;
      for (j = index; j < pn; j++)
	{
	  if (fr[i] == pg[j])
	    {
	      if (j > farthest)
		{
		  farthest = j;
		  res = i;
		}
	      break;
	    }
	}

      // If a page is never referenced in future,
      // return it.
      if (j == pn)
	return i;
    }

  // If all of the frames were not in future,
  // return any of them, we return 0. Otherwise
  // we return res.
  return (res == -1) ? 0 : res;
}

void
optimalPage (int pg[], int pn, int fn)
{
  // Create an array for given number of
  // frames and initialize it as empty.
  vector < int >fr;

  // Traverse through page reference array
  // and check for miss and hit.
  int hit = 0;
  for (int i = 0; i < pn; i++)
    {

      // Page found in a frame : HIT
      if (search (pg[i], fr))
	{
	  hit++;
	  continue;
	}

      // Page not found in a frame : MISS

      // If there is space available in frames.
      if (fr.size () < fn)
	fr.push_back (pg[i]);

      // Find the page to be replaced.
      else
	{
	  int j = predict (pg, fr, pn, i + 1);
	  fr[j] = pg[i];
	}
    }
  cout << "No. of Page Faults = " << pn - hit << endl;
}

int *
generateReferenceString (int size_pg)
{

  srand (time (NULL));
  int *pointer;
  int ReferenceString[size_pg];
  pointer = ReferenceString;
  for (int i = 0; i < size_pg; i++)
    {
      ReferenceString[i] = rand () % 7 + 1;
    }
  return pointer;
}

// Driver Function
int
main ()
{

  int number_frames, size_pg;
  printf ("Enter number of frames: ");
  scanf ("%d", &number_frames);
  printf ("Frames: %d", number_frames);
  printf ("\n");
  printf ("Enter size of Page Reference String: ");
  scanf ("%d", &size_pg);
  printf ("PRS size: %d ", size_pg);
  printf ("\n");

  int *pages = generateReferenceString (size_pg);
  printf ("The randomized Reference String: ");
  for (int i = 0; i < size_pg; i++)
    {
      printf ("%d ", pages[i]);
    }

  printf ("\n");
  int pn = sizeof (pages) / sizeof (pages[0]);
  printf ("OPTIMAL PAGE REPLACEMENT ALGORITHM");
  printf ("\n");
  optimalPage (pages, pn, number_frames);
  printf ("LRU REPLACEMENT ALGORITHM");
  printf ("\n");
  int lrufaults = lruPage (pages, pn, number_frames);
  printf ("No. of Page Faults = %d ", lrufaults);
  printf ("\n");
  printf ("FIFO REPLACEMENT ALGORITHM");
  printf ("\n");
  int fifofaults = fifoPage (pages, pn, number_frames);
  printf ("No. of Page Faults = %d ", fifofaults);
  return 0;

}
