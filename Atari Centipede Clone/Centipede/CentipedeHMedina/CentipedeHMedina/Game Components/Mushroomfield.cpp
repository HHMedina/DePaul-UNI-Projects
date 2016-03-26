#include "Mushroomfield.h"
#include "Mushroom.h"
#include "RecyclingMushroomFactory.h"
#include <math.h>
#include "WaveManager.h"
#include "EnemyExplosion.h"
#include "RecyclingEnemyExplosionFactory.h"

#include "PlayerManager.h"
#include "Player.h"
#include "Player2.h"
#include "Player1.h"
int Mushroomfield::NumberOfMushroomFields=0;

Mushroomfield::Mushroomfield(){
	GridStart = sf::Vector2f(MUSHROOMSEGMENT/2,(MUSHROOMSEGMENT/2)+MUSHROOMSEGMENT);
	AmountOfMushrooms =0;
	AmountOfPlayerSideMushrooms=0;
	//sets Array elements to NULL at start due to c++ not having an
	//array of objects instantiated with NULLs;
	for(int i =0; i<rows;i++){
		for(int j =0;j<columns; j++){
			Field[i][j] =NULL;
			StateField[i][j] = false;
		}
	}
	++NumberOfMushroomFields; //used to create different random fields per player

	MushroomFieldNumber=NumberOfMushroomFields;
}

void Mushroomfield::Initialize(int NumberOfMushrooms){

	for(int i =0; i<rows;i++){
		for(int j =0;j<columns; j++){
			//if there is a mushroom in the field then destroy it
			if(Field[i][j]!=NULL){
				Field[i][j]->MarkForDestroy();
				Field[i][j] =NULL;
				
			}
			//sets the bool field to false
			if(StateField[i][j]=true){
				StateField[i][j] = false;
			}
		}
	}


	//initialize flea count number and amount of mushrooms on field
	AmountOfMushrooms =0;
	AmountOfPlayerSideMushrooms=0;


	int randRow;
	int randColumn;
	srand(MushroomFieldNumber+time(NULL));
	int counter=0;

	//Populates the grid randomly and checks if there is a mushroom
	//in the random spot. Do nothing if this is the case.
	//prevents mushrooms being on top of each other.
	// Note: If the number of mushrooms to be instantiated is larger than 
	//the number of elements the array can hold, the number of instantiated
	//Mushrooms will be the rows*columns (full grid)
	if(NumberOfMushrooms>rows*columns){
		NumberOfMushrooms = rows*columns;
	}
	for(counter;counter<NumberOfMushrooms;){
		randRow = rand()%rows;
		randColumn = rand()%columns;
		if(StateField[randRow][randColumn]==false){
			StateField[randRow][randColumn] = true;
			++counter;
			++AmountOfMushrooms;
			if(randRow>=StartOfPlayerSideMushroomArea&&randRow<=rows){
				++AmountOfPlayerSideMushrooms;
			}
		}
		
		
		
	}
	

}

sf::Vector2i Mushroomfield::WorldPositionToMushroomGrid(const sf::Vector2f worldPosition) const{
	sf::Vector2i coordinates;
	//if not within the grid, return 31's which will cause Checkgridspace to return false
	//when regarding the centipede checking for cells outside of the mushroomgrid
	if(worldPosition.y<GridStart.y||worldPosition.y>GridStart.y+(MUSHROOMSEGMENT*(rows-1))||worldPosition.x<MUSHROOMSEGMENT/2||worldPosition.x>WindowManager::MainWindow.getView().getSize().x-MUSHROOMSEGMENT/2){
	coordinates.x = 31;
	coordinates.y = 31;
	return coordinates;
	}
	
	coordinates.y = floor(worldPosition.x/MUSHROOMSEGMENT);
	coordinates.x = floor((worldPosition.y-GridStart.y)/MUSHROOMSEGMENT);

	
	return coordinates;
}

void Mushroomfield::DestroyMushroom(const sf::Vector2i grid){

	//calls destroy method which called delete on Mushroom
	Field[grid.x][grid.y]->MarkForDestroy();
	Field[grid.x][grid.y]=NULL;
	StateField[grid.x][grid.y] =false;
	--AmountOfMushrooms;

	if(grid.x>=StartOfPlayerSideMushroomArea&&grid.x<=rows){
     	--AmountOfPlayerSideMushrooms;
		WaveManager::CheckToSpawnFlea();
	}

}

bool Mushroomfield::CheckMushroomForPoison(const sf::Vector2i gridTarget) const{
	if(gridTarget.x>rows-1||gridTarget.x<0||gridTarget.y<0||gridTarget.y>columns-1){
		return false;
	}else if(Field[gridTarget.x][gridTarget.y]==NULL){
		return false;
		
	}else if(Field[gridTarget.x][gridTarget.y]->IsPoisoned){
		
		return true;
		
	}
 	
	return false;

}
bool Mushroomfield::CheckGridSpace(const sf::Vector2i gridTarget) const{
	//returns false if not checking within the grid's range
	if(gridTarget.x>rows-1||gridTarget.x<0||gridTarget.y<0||gridTarget.y>columns-1){
		return false;
	}

	if(Field[gridTarget.x][gridTarget.y] == NULL){
		return false;
	}
	return true;
}

Mushroom& Mushroomfield::AccessMushroom(const sf::Vector2i gridCoordinates)const{

	return *Field[gridCoordinates.x][gridCoordinates.y];
}
//creates a mushroom at the assigned vectori coordinates (does not check if there
//is already a mushroom in the spot. Use CheckGridSpace for checking if there is already a mushroom in the location.
void Mushroomfield::CreateMushroomAtCoordinate(const sf::Vector2i& gridCoordinates){
	
	//if mushroom is created in playerarea increment amount of mushrooms in player area
	if(gridCoordinates.x>=24&&gridCoordinates.x<=rows){
		++AmountOfPlayerSideMushrooms;
		
	}

	if(gridCoordinates.x<=rows&&gridCoordinates.y<=columns){//checks if the desired location is within the Mushroom grid
		Field[gridCoordinates.x][gridCoordinates.y] =&RecyclingMushroomFactory::CreateMushroom(sf::Vector2f(GridStart.x+(MUSHROOMSEGMENT*gridCoordinates.y),GridStart.y+(MUSHROOMSEGMENT*gridCoordinates.x)),gridCoordinates,*this);
		StateField[gridCoordinates.x][gridCoordinates.y] = true;
		++AmountOfMushrooms;
	}
}

void Mushroomfield::HealAllMushrooms(){

	int Counter=0;
	for(int i =0; i<rows;){
		for(int j =0;j<columns;){
			
			//if there is a mushroom in that spot
			if(Field[i][j]!=NULL&&(Field[i][j]->HP<Mushroomfield::MaxMushroomHP||Field[i][j]->IsPoisoned)){
					RecyclingEnemyExplosionFactory::CreateEnemyExplosion(Field[i][j]->WorldPosition);
					Field[i][j]->FullyHeal();//heal the mushroom
					++Counter;//increment counter
					if(Counter>=NumberOfMushroomsHealedBeforeGettingPoints){//player gets 5 points for each 2 damaged mushrooms
						PlayerManager::GetPlayer()->AddToPlayerScore(ScoreRecievedPerEachBundleOfMushroomsHealed);
						Counter=0;//reset the counter
					}
				}
				++j;
		}
		++i;
	}


	WaveManager::EndOfWaveDecision(); // either spawn another time //single player or spawn the next player.
}

void Mushroomfield::Alarm0(){

	ConsoleMsg::WriteLine("Alarm0 activated");
}

void Mushroomfield::ClearTheField(){

	//clears the physical field but not the state of the field
	for(int i =0; i<rows;i++){
		for(int j =0;j<columns; j++){
			if(Field[i][j]!=NULL){
				Field[i][j]->MarkForDestroy();
				Field[i][j] =NULL;
			}
		}
	}

}

void Mushroomfield::RespawnField(){

	for(int i =0; i<rows;i++){
		for(int j =0;j<columns; j++){
			if(StateField[i][j]==true){
				if(Field[i][j]==NULL){
					Field[i][j] = &RecyclingMushroomFactory::CreateMushroom(sf::Vector2f(GridStart.x+(MUSHROOMSEGMENT*j),GridStart.y+(MUSHROOMSEGMENT*i)),sf::Vector2i(i,j),*this);//new Mushroom(GridStart.x+(MUSHROOMSEGMENT*randRow),GridStart.y+(MUSHROOMSEGMENT*randColumn),sf::Vector2i(randRow,randColumn), *this);
				}
			}
		}
	}

}

