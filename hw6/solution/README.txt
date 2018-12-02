Allen Tran

Description of what we have to do :

Implement the following in C++ :
	[x] A class that handles JSON tasks
		[] Deserialize
			Read in a file, then
			Create an instance of state of the game class
		[] Serialize
			Read in the state of the game class, then
			Write to a file
	[x] A class that represents the state of the game
                uses the boardState struct in it's initialization
		We build upon our hw3 code
		[] Implement standard class fcn's
			[] get
			[] set
			[] etc (feel free to add on)
		We must accomedate for all the info given by
		our JSON file
	[x] A general board class ( there could be a better way )
		Carbon copy of our array2d class
		We do this so that we want to overwrite swap
	[x] A subclass that represents the ACTUAL BOARD
		We build upon our array2d class. Big difference is that we
		have to handle 'Templates' and 'Gravity' on this board. So
		we will have to implement the methods here.
		[] fields 
			int array that points to curr_selected
			array2dPtr holder, marked
		[] Fcn's
			[] getRow, getCol
			[] Updated Swap
				Have to check :	
				**Does a check to see if valid before cont.**
				If (valid) then 
					Decrement "Jelly" count, 
					Swap (array2d version),
					Proceed to Scan Template
				Else 
					Dont swap (& give user error msg?)

			[] Scan templates *** may be difficult
				Start with highest priority template
				nested loop from (0,0) to end
					if (match) then call 'candy_explosion'
					(more detail below)
				loop until all templates are used
			[] candy_explosion : redraws board
				We have to specify a convention for passing
				parameters into this fcn in-order for it to
				"explode" the right candies. We want to mark
				the exploded candies so we can "fill in" with
				'Gravity'.
				---------------------------------------------
				Current ideas on convention :
				Have two primary fcns, for horizontal and for 
				veritcal explosions
				Pass in the (coordinates) of the leftmost or
				topmost of the chain/template.
				Pass in the template or chain number
					we read left-right, top-bottom

				Mark those candies (for clean-up)
				Call a clean-up/update board fcn
				---------------------------------------------	
				Comments :
				
				---------------------------------------------
			[] Clean-up/update board fcn
				We will recursively fill in the 'marked' 
				candy slots until none are left
				We will traverse a bit differently ere
				For each column : traverse each row
					look for 'marked'
					do in place recursive swap down
					**Have to keep track of extendOffset**
					continue from idx  until hit last row
				continue loop until hit last column
				**Might have to pause User Input until finish**
			[] Decrement Jelly Count
				I dont necessarily want to have a third 
				array2d struct to hold this tiny bit of info.
				What else can we do?
				---------------------------------------------
				Idea : 
				Initialize board with customized candy images
				(change saturation/hue/etc) to indicate
				that it is a candy/block we need to get rid
				of to clear the stage.
				Then when it is 'marked' we use the standard
				candy images to overwrite,etc

				Idea 2 :
				We minimize the space used
				Make a bit array storing 0's and 1's
				Where the values are defined as,
				1 - needs to be destroyed
				0 - destroyed already		
				---------------------------------------------
				Comments :
				Allen - We can do both, Idea 1 is needed 
					unless we find a better way to 
					indicate to the user these special
					blocks.
				
				---------------------------------------------
		[x] A subclass that represents the EXTENDED BOARD
			Fields
       				[] all fields that the struct contains
				[] array2d holder
				[] int[] that represents curr_offset
			Fcns 
				[] incr_Offset(int column);
extended board will now contain all the information in the struct and have it
passed by reference so that when the fields in the extendedboard class that are
associated with the struct field are modified, the struct will also get modified
done because want to keep hw3 code as is, easy fix
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		[] A wrapper class
			Holds 
				GAME ID ( char * )
				ACTUAL BOARD
				EXTENDED BOARD			
				SCORE ( details below )
				-----------------------
				COLORS (int)
				(feel free to add onto this)
			Fcns
				[] get
				[] set
				[] etc (feel free to add on)
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	[] Optional topic, Handling Signals
		On exit, we can try to catch the "exit signal" and call
		our serialize or just make a button for that.

################################# LOG ######################################
Allen - added a indepth structure to get started, feel free to add on/ comment
	just add a comment box like I did above and tag your name in front
	of your message.
