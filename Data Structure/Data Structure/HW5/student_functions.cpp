#include <fstream>
#include <iostream>
#include <string>
#include "SongLibrary.h"

//////////////////DO NOT CHANGE THIS CODE////////////////////////

//Construct a Song object with values
Song::Song(const std::string& title, const std::string& artist, 
     const std::string& album, unsigned int year){
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->year = year;
	used = false;
	this->song_group_ptr = NULL;
}

//Song equality doesn't depend on pointers or being used
bool operator== (const Song& s1, const Song& s2){
	return (s1.getYear() == s2.getYear()) && (s1.getTitle() == s2.getTitle())
          && (s1.getAlbum() == s2.getAlbum()) && (s1.getArtist() == s2.getArtist());
}

//Modifier for group pointer
void Song::updateSongGroupPointer(SongGroupNode* ptr){
	if(ptr){
		//Check that ptr is actually for this Song.
		assert(ptr->m_song->value == *this); 
		song_group_ptr = ptr;
	}
	else{
		song_group_ptr = NULL;
	}
}

//Make output easier by overload operator<<
std::ostream& operator<< (std::ostream& ostr, const Song& song){
	if(!song.getSongGroupPointer()){
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
	       << song.getAlbum() << "\" (" << song.getYear() << ")" << std::endl;
	}
	else{
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
		<< song.getAlbum() << "\" (" << song.getYear() << ")" << " in Song Group "
		<< song.getSongGroupPointer()->id << std::endl;
	}
	return ostr;
}

/////////////////////////YOUR CODE STARTS HERE/////////////////////////////////


//////////////////////////////GROUP FUNCTIONS/////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are searching for
BEHAVIOR:
None
RETURN:
NULL if no list matches the ID, otherwise returns the GroupListNode* containing
the head of the list with ID id.
*/
GroupListNode* GroupExists(GroupListNode* song_groups, GroupID id)
{
	//Go through every group
	for(GroupListNode *p = song_groups; p != NULL; p = p->ptr )
	{
		SongGroupNode *l = p -> value;
		if ((l->id) == id)
			return p;
		
	}
	return NULL;




}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are creating
BEHAVIOR:
Add a new song group to song_groups, give the new song group ID id. Order of
song groups is not guaranteed.
RETURN:
None
*/
void MakeGroup(GroupListNode*& song_groups, GroupID id)
{
		//if it is an empty list
		if(song_groups == NULL)
		{	
			song_groups = new GroupListNode;
			song_groups -> value = new SongGroupNode;
			song_groups -> value-> id = id; 
			song_groups->value -> next_song_ptr = NULL;
			song_groups->value -> next_by_artist_ptr =NULL;
			song_groups->value -> prev_song_ptr = NULL;
			song_groups->value -> prev_by_year_ptr = NULL;
			song_groups->value -> m_song = NULL;
			song_groups -> ptr = NULL;
		}
		//If it is not empty, then push to the last.
		else
		{
			GroupListNode *p = song_groups;
			while(p->ptr !=NULL)
				p = p->ptr;
			p->ptr = new GroupListNode;
			p->ptr -> value = new SongGroupNode;
			p->ptr -> value-> id = id; // new ?
			p->ptr->value -> next_song_ptr = NULL;
			p->ptr->value -> next_by_artist_ptr =NULL;
			p->ptr->value -> prev_song_ptr = NULL;
			p->ptr->value -> prev_by_year_ptr = NULL;
			p->ptr->value -> m_song = NULL;
			p->ptr -> ptr = NULL;

		}
		

	
}


/*
ARGUMENTS:
-group_head points to the head of a song group
-song is the Song we are checking the group for
BEHAVIOR:
None
RETURN:
If song is found in the group return true. If song is not found,
return false.
*/
bool SongInGroup(SongGroupNode* group_head, const Song& song)
{	
	//Go through every song in group.
	for(SongGroupNode* p = group_head; p!= NULL; p = p -> next_song_ptr)
	{
		if((p->m_song->value) == song)
			return true;
	}
	return false;




}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to add.
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Modify the group pointed to by list_ptr to reflect that the song is in the group.
This may change the length of the list. Modifies the Song's members as well.
RETURN:
None
*/
void AddToGroup(LibraryNode* library_ptr, GroupListNode* list_ptr)
{
	
	SongGroupNode* m = list_ptr -> value;
	library_ptr->value.setUsedInGroup(true);
	//If this group list is empty
	if(list_ptr -> value -> m_song == NULL)
	{
		list_ptr->value->m_song = library_ptr;
		library_ptr->value.updateSongGroupPointer(list_ptr->value);

	}
	//If the group list is not empty, then just push back.
	else
	{
		while(m->next_song_ptr != NULL)
		{m = m->next_song_ptr;}
		SongGroupNode* add_song;
		add_song = new SongGroupNode;
		add_song-> id = m->id; // new ?
		add_song-> next_song_ptr = NULL;
		add_song-> next_by_artist_ptr =NULL;
		add_song-> prev_song_ptr = m;
		add_song-> prev_by_year_ptr = NULL;
		add_song->m_song = library_ptr;
		m->next_song_ptr = add_song;
		library_ptr->value.updateSongGroupPointer(m->next_song_ptr);
		//Change the previous next_by_artist_ptr
		std::string a_name = library_ptr->value.getArtist();
		for(; m!=NULL; m = m->prev_song_ptr)
		{
			if((m->m_song->value.getArtist() == a_name) && (m->next_by_artist_ptr == NULL))
			{
				m->next_by_artist_ptr = add_song;
			}
		}

		RecomputePrevByYear(list_ptr);
	}


}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Modifies the Song's member variables to reflect that it is not in a group. Modifies the group
so that the Song is not in the group. This function may cause song_groups to be modified as well.
RETURN:
None
*/
void RemoveFromGroup(LibraryNode* library_ptr, GroupListNode* song_groups)
{
	//Use a big while loop to go through 

				SongGroupNode* l = library_ptr->value.getSongGroupPointer();
				
				//If the first one and the only one
				if(l->prev_song_ptr == NULL && l->next_song_ptr == NULL)
				{
					l->m_song = NULL;
				}
				//If it is the lastone
				else if(l->prev_song_ptr != NULL && l->next_song_ptr == NULL)
				{
					l->prev_song_ptr->next_song_ptr = NULL;
					//update next by artist here
					for(SongGroupNode* temp = l; temp!=NULL; temp = temp->prev_song_ptr)
					{
						if(temp->next_by_artist_ptr == l)
						{
							temp->next_by_artist_ptr = NULL;
						}
					}
					delete l;
				}
				//If it is the first one.
				else if(l->prev_song_ptr == NULL && l->next_song_ptr != NULL)
				{
					l->next_song_ptr->prev_song_ptr = NULL;
					
					//updating the prevbyyear and change the head pointer.
					for(GroupListNode*p = song_groups; p != NULL; p = p->ptr)
					{
						if(p->value->id == l->id)
						{
							p->value = l->next_song_ptr;
							RecomputePrevByYear(p);

						}
					}

			
					delete l;
				}
				//It is in the middle.
				else
				{
					//update next by artist here
					for(SongGroupNode* art_temp = l; art_temp!=NULL; art_temp = art_temp->prev_song_ptr)
					{
						if(art_temp->next_by_artist_ptr == l)
						{
							art_temp->next_by_artist_ptr = l->next_by_artist_ptr;
						}
					}

					l->prev_song_ptr->next_song_ptr = l->next_song_ptr;
					l->next_song_ptr->prev_song_ptr = l->prev_song_ptr;
					//updating the prevbyyear.
					for(GroupListNode*p = song_groups; p != NULL; p = p->ptr)
					{
						if(p->value->id == l->id)
						{
							RecomputePrevByYear(p);

						}
					}
					delete l;

				}
				library_ptr->value.updateSongGroupPointer(NULL);
				library_ptr->value.setUsedInGroup(false);
				
				
}
		

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr1 contains to the song group head of Group A
-list_ptr2 contains the song group head of Group B
BEHAVIOR:
Modify Group A so that at the end of its songs, the songs of Group B are added in the same
order they were found in Group B. Remove the now-empty Group B.
RETURN:
None
*/
void CombineGroups(GroupListNode*& song_groups, GroupListNode*& list_ptr1, GroupListNode*& list_ptr2)
{
	SongGroupNode* p = list_ptr1->value;
	//Get the last node for list1
	while(p->next_song_ptr!=NULL)
	{
		p = p->next_song_ptr;
	}
	//Change the id for list2
	for(SongGroupNode* m = list_ptr2->value; m!=NULL;m = m->next_song_ptr)
	{
		m->id = list_ptr1->value -> id;
	}
	//Connect two list
	p->next_song_ptr = list_ptr2->value;
	list_ptr2->value->prev_song_ptr = p;
	//Delete the Groupb
	//if the group to be delete is the start of group
	if(song_groups == list_ptr2)
	{
		song_groups = list_ptr2->ptr;
		delete list_ptr2;
	}
	//if the group to be deleted is the end of the group
	else if(list_ptr2->ptr == NULL)
	{
		GroupListNode*t = song_groups;
		while(t->ptr->ptr!=NULL)
		{
			t = t->ptr;
		}
		t->ptr = NULL;
		delete list_ptr2;
	}

	//if the group to be deleted is in the middle.
	else
	{
		GroupListNode*t = song_groups;
		while(t!=NULL)
		{
		
			if(t->ptr == list_ptr2)
			{
				t->ptr = list_ptr2->ptr;
				delete list_ptr2;
				break;
			}
			t = t->ptr;
		}
	}
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
BEHAVIOR:
Remove the group from the list of song groups. No Song objects should remain associated with the
group, and the function should modify Song member variables appropriately to reflect this. Any
memory associated with the group must be released.
RETURN:
None
*/
void RemoveGroup(GroupListNode*& song_groups, GroupListNode* list_ptr)
{	
	//Delete every song  in the group.
	for(SongGroupNode *p = list_ptr->value; p != NULL;)
	{
		SongGroupNode* temp = p->next_song_ptr;
		p->m_song->value.updateSongGroupPointer(NULL);
		p->m_song->value.setUsedInGroup(false);
		delete p;
		p = temp;

	}
	
	//Start delete Group
	//If the group is the first 
	if(song_groups == list_ptr)
	{
		song_groups = song_groups->ptr;
		delete list_ptr;
		list_ptr = song_groups;
	}

	//If the group is the last
	else if(list_ptr ->ptr ==NULL)
	{
		GroupListNode *p = song_groups;
		while(p->ptr->ptr !=NULL)
		{
			p = p->ptr;
		}
		p ->ptr = NULL;
		delete list_ptr;
		

	}
	//Middle groups
	else
	{
		GroupListNode *p = song_groups;
		GroupListNode *p1;
		
		for(;p!=NULL; p = p->ptr)
		{
			if(p->ptr->value->id == list_ptr->value->id)
			{
				p1 = p;
			}
		}

		p1->ptr = list_ptr -> ptr;
		delete list_ptr;
		list_ptr = p1->ptr;
	}



}

/*
ARGUMENTS:
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Recompute all prev_by_year_ptr members in the song group pointed to by
list_ptr.
RETURN:
None
*/
void RecomputePrevByYear(GroupListNode* list_ptr)
{
	//Go through every song
	for(SongGroupNode*p = list_ptr->value; p !=NULL; p = p ->next_song_ptr)
	{
		//Check back
		for(SongGroupNode*l = p; l !=NULL;l = l->prev_song_ptr)
		{
			p->prev_by_year_ptr = NULL;
			if(l->m_song->value.getYear() < p->m_song->value.getYear())
			{
				p->prev_by_year_ptr = l;
				break;
			}
		}
	}



}

////////////////////////////PRINT FUNCTIONS////////////////////////////////////

/*
ARGUMENTS:
-library_head points to the first LibraryNode* of the song library
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the library in the order it was added.
RETURN:
None
*/
void PrintLibrary(LibraryNode* library_head,std::ostream& print_file){
	int count = 0;
	//Go through library.
	for(LibraryNode* p = library_head; p!=NULL; p = p->ptr)
	{
		count++;
		print_file << p->value;
	}
	print_file << "Library contains " << count << " song(s)" << std::endl;




}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to print
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the group in the order it was added.
RETURN:
None
*/
void PrintGroupSequential(SongGroupNode* group_head,std::ostream& print_file){
	int count = 0;
	SongGroupNode *p = group_head;
	//if no group element.
	if(p->m_song == NULL)
		print_file << "List ID "<< p->id <<" contains:\n" <<"0 song(s)" << std::endl;

	else
	{	
		print_file << "List ID "<< p->id <<" contains:" << std::endl;
		for(; p != NULL; p = p->next_song_ptr)
		{
			count++;
			print_file << p->m_song ->value;
		}
		print_file << count << " song(s)" << std::endl;
	}


}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the prev_by_year_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupRewind(SongGroupNode* group_ptr,std::ostream& print_file){
print_file<<"Rewind list for " << group_ptr->m_song->value.getYear() <<std::endl;
int count = 1;
//Go through the group songs
for(;group_ptr!=NULL; group_ptr = group_ptr->prev_by_year_ptr)
{
	print_file<<"\t" <<"#" <<count <<": " << group_ptr->m_song->value;
	count++;
}
print_file << "Rewind list contained " << count-1 << " song(s)." << std::endl;



}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the next_by_artist_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupMarathon(SongGroupNode* group_ptr,std::ostream& print_file){
print_file<<"Marathon list for " << group_ptr->m_song->value.getArtist() <<std::endl;
int count = 1;
//Go through the group songs
for(;group_ptr!=NULL; group_ptr = group_ptr->next_by_artist_ptr)
{
	print_file<<"\t#" <<count<<": " <<group_ptr->m_song->value;
	count++;
}
print_file << "Marathon list contained " << count-1 << " song(s)." << std::endl;

}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to analyze
-print_file points to an ostream to print output to
BEHAVIOR:
Find the rewind list with the most songs in the song group pointed to by group_head.
Print the rewind list and the song count to print_file.
RETURN:
None
*/
void PrintLongestGroupRewind(GroupListNode* group_head,std::ostream& print_file){
	SongGroupNode *max;
	int maxcount = 0;
	//Get the longest node!
	for(SongGroupNode * p = group_head->value ; p!=NULL;p = p->next_song_ptr)
	{
		int count = 0;
		for(SongGroupNode *l = p; l!=NULL; l = l->prev_by_year_ptr)
		{
			count++;
		}
		if(count > maxcount)
		{
			max = p;
		}
	}

	//printing
	print_file<<"Maximum size rewind list is for year " << max->m_song->value.getYear() <<std::endl;
	int count = 1;
	for(;max!=NULL; max = max->prev_by_year_ptr)
	{
		print_file<<"\t" <<"#" <<count<<": " <<max->m_song->value;
		count++;
	}
	print_file << "Longest rewind list contained " << count-1 << " song(s)." << std::endl;


}

///////////////////////////////CLEANUP/////////////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Delete every group. Do not leak. Modifies all groups and the song_groups list.
Update every Song to reflect not being in a group.
RETURN:
None
*/
void DeleteAllSongGroups(GroupListNode*& song_groups){
if(song_groups)
{
	//If there's something to remove
		GroupListNode*p = song_groups; //Save a copy of current head
		song_groups = song_groups->ptr; //Make head point to the next thing
		RemoveGroup(p,p);
		DeleteAllSongGroups(song_groups);
}


}
