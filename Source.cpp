#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

struct Car
{
	int moveCounter = 0;
	string licensePlate = " ";
	
	void addMove()
	{
		moveCounter =+ 1;

	}
};


struct Node
{
	Car carData;
	Node* next = NULL;
	Node(Car data) {
		carData = data;
	}
};


class Stack
{
private:
	Node* top;
	Node* bottom;
	int capacity = 10;
	int spotsOccupied = 0;
public:
	Stack()
	{
		top = NULL;
		bottom = NULL;
	}

	void prependCar(Car data);
	void removeCar();
	bool isFull();
	bool isEmpty();
	Car topCar();
	bool findCar(Car);
	void traverseEntireLane();

};


class Garage
{
private:
	Stack lane1;
	Stack lane2;
	Stack street;

public:
	void carArrives(Car); //calls prepend function
	void carDeparts(Car); //calls pop function
	void moveCar(Stack&, Stack&);
	void clearStreet();
	void displayAllCars();


};



//MAIN---------------------------------------------------
int main()
{
	Garage wow;

	Car BigShaq; //the one and only. Mans not hot...

	char code = ' '; //check if 'A' or 'D'
	string licensePlate = " ";
	//bool endCase = false;
	ifstream infile;
	infile.open("file.txt");

	while (infile >> code)
	{
		if (code == 'A')
		{
			infile >> BigShaq.licensePlate;
			wow.carArrives(BigShaq);
			wow.displayAllCars();
		}
		else if (code == 'D')
		{
			infile >> BigShaq.licensePlate;
			wow.carDeparts(BigShaq);
			wow.displayAllCars();
		}
	}
	
	infile.close();
	return 0;
}



//STACK---------------------------------------------------
void Stack::prependCar(Car data)
{
	if (spotsOccupied < 10)
	{
		Node* newNode = new Node(data);
		if (top == NULL)
		{
			top = newNode;
			bottom = newNode;
			spotsOccupied++;
			
		
			return;
			
		}

		else
		{
			newNode->carData = data;
			newNode->next = top;
			top = newNode;
			spotsOccupied++;
			
			return;
		
		}
	}

	else
	{
		cout << "Lane is full" << endl;
		return;
	}

}


void Stack::removeCar()
{
	if (top == NULL)
	{
		/*cout << "Lane is empty" << endl << endl;*/
		return;
	}
	else
	{
		Node* temp = top;
		top = top->next;
		delete temp;
		spotsOccupied--;
		
		return;
	}
	//counter++
}


bool Stack::isEmpty() 
{
	if (spotsOccupied == 0)
		return true;
	else
		return false;

}


bool Stack::isFull()
{
	if (spotsOccupied == capacity)
		return true;
	else
		return false;

}


Car Stack::topCar()
{
	if (isEmpty())
	{
		Car temp;
		temp.licensePlate = "ERROR";
		return temp;
	}
	else
	{
		return top->carData;
	}

}


bool Stack::findCar(Car data)
{
	Node* current = top;
	while (current != NULL)
	{
		if (current->carData.licensePlate == data.licensePlate)
		{
			return true;
		}
		current = current->next;
	}
	return false;

}


void Stack::traverseEntireLane()
{
	Node* current = top;
	while (current != NULL)
	{
		cout << current->carData.licensePlate << ": " << current->carData.moveCounter << " moves| ";
		current = current->next;
	}

}



//GARAGE---------------------------------------------

void Garage::carArrives(Car newcar)
{
	if (!lane1.isFull()) //park if lane 1 is empty or is not full
	{
		cout << "============================================" << endl;
		cout << "PARKING A NEW CAR IN LANE 1 ==>> " << newcar.licensePlate << endl;
		lane1.prependCar(newcar);
		return;
	}
	else if (lane2.isEmpty() || !lane2.isFull()) //park in lane 2 if it's empty of not full
	{
		cout << "============================================" << endl;
		cout << "PARKING A NEW CAR IN LANE 2 ==>> " << newcar.licensePlate << endl;
		lane2.prependCar(newcar);
		return;
	}
	else if (lane1.isFull() && lane2.isFull()) //both lanes are fully occupied
	{
		cout << "============================================" << endl;
		cout << "Sorry! The garage is fully occupied." << endl;
		return;
	}

}


void Garage::carDeparts(Car searchCar)
{
//---------------------if car is found in lane 1, depart car
	if (lane1.findCar(searchCar) == true)
	{
		if (lane1.topCar().licensePlate == searchCar.licensePlate) //if searchCar is at the top, depart searchCar
		{
			cout << "============================================" << endl;
			cout << "A CAR IS DEPARTING ==>> " << lane1.topCar().licensePlate << ": Was moved " << lane1.topCar().moveCounter << " times" << endl;
			lane1.removeCar();
			//displayAllCars();
			return;
		}
	//------------------------------------------------------------------------------------------------------
		else //if searchCar is not at the top, 
		{
			while ((!lane2.isFull()) && (lane1.topCar().licensePlate != searchCar.licensePlate)) //loop to check if lane 2 is not full and the top car still does not match the searchCar
			{
				moveCar(lane1, lane2);
				lane2.topCar().addMove();
				if (lane1.topCar().licensePlate == searchCar.licensePlate)
				{
					cout << "============================================" << endl;
					cout << "A CAR IS DEPARTING ==>> " << lane1.topCar().licensePlate << ": Was moved " << lane1.topCar().moveCounter << " times" << endl;
					lane1.removeCar();
					while (!lane2.isEmpty() && !lane1.isFull())
					{
						moveCar(lane2, lane1);
						lane1.topCar().addMove(); //add +1 to move counter of car
						
					}
					return;
				}
			}
			while (!lane1.isFull() && (!lane2.isEmpty() || !street.isEmpty())) //if lane 1 is not full, move cars from lane 2 to lane 1 first. Then from street to lane 1 or lane 2
			{
				if (!lane2.isEmpty())
				{
					moveCar(lane2, lane1); //if lane 2 still has cars, move them to lane 1
					lane1.topCar().addMove(); //add +1 to move counter of car

				}
				else if (!lane1.isEmpty() && lane2.isEmpty() && !street.isEmpty())
				{
					moveCar(street, lane1); //if lane 2 has no cars, lane 1 is not yet filled, and street still has cars, move cars from street to lane 1
					lane1.topCar().addMove(); //add +1 to move counter of car

				}
				else if (lane1.isFull() && !lane2.isFull() && !street.isEmpty())
				{
					moveCar(street, lane2); //if lane 1 is full, but lane 2 is not and there are still cars in the street, move from street to lane 2
					lane2.topCar().addMove(); //add +1 to move counter of car

				}
			}
			return;
			
			while (lane1.topCar().licensePlate != searchCar.licensePlate) //when lane 2 is full, loop to move cars to street until searchCar is found and departed
			{
				moveCar(lane1, street);
				street.topCar().addMove();
				if (lane1.topCar().licensePlate == searchCar.licensePlate)
				{
					cout << "============================================" << endl;
					cout << "A CAR IS DEPARTING ==>> " << lane1.topCar().licensePlate << ": Was moved " << lane1.topCar().moveCounter << " times" << endl;
					lane1.removeCar();
					clearStreet();
					
					return;
				}
			}

			while (!lane1.isFull() && (!lane2.isEmpty() || !street.isEmpty())) //if lane 1 is not full, move cars from lane 2 to lane 1 first. Then from street to lane 1 or lane 2
			{
				if (!lane2.isEmpty())
				{
					moveCar(lane2, lane1); //if lane 2 still has cars, move them to lane 1
					lane1.topCar().addMove(); //add +1 to move counter of car

				}
				else if (!lane1.isEmpty() && lane2.isEmpty() && !street.isEmpty())
				{
					moveCar(street, lane1); //if lane 2 has no cars, lane 1 is not yet filled, and street still has cars, move cars from street to lane 1
					lane1.topCar().addMove(); //add +1 to move counter of car

				}
				else if (lane1.isFull() && !lane2.isFull() && !street.isEmpty())
				{
					moveCar(street, lane2); //if lane 1 is full, but lane 2 is not and there are still cars in the street, move from street to lane 2
					lane2.topCar().addMove(); //add +1 to move counter of car

				}
			}
			return;
		}
	}
//---------------------if car is found in lane 2, depart car
	else if (lane2.findCar(searchCar) == true)
	{
		if (lane2.topCar().licensePlate == searchCar.licensePlate) //if searchCar is at the top, depart searchCar
		{
			lane2.removeCar();
			return;
		}
		//------------------------------------------------------------------------------------------------------
		else //if searchCar is not at the top, 
		{
			while (lane2.topCar().licensePlate != searchCar.licensePlate)
			{
				moveCar(lane2, street);
				if (lane2.topCar().licensePlate == searchCar.licensePlate) //
				{
					lane2.removeCar();
					clearStreet();
					return;
				}
			}
		}
	}
//---------------------searchCar is neither in lane 1 nor lane 2
	else if (lane1.findCar(searchCar) == false && lane2.findCar(searchCar) == false)
	{
		cout << "============================================" << endl;
		cout << "FAILED TO DEPART \"" << searchCar.licensePlate << "\" AS IT IS NOT IN THIS GARAGE" << endl;
		return;
	}
}


void Garage::moveCar(Stack& origin, Stack& destination)
{

	destination.prependCar(origin.topCar());
	destination.topCar().addMove(); //not working
	origin.removeCar();

}


void Garage::clearStreet()
{
	while (!street.isEmpty())
	{
		if (!lane1.isFull())
		{
			moveCar(street, lane1);
			lane1.topCar().addMove();
		}
		else
		{
			moveCar(street, lane2);
			lane2.topCar().addMove();
		}
	}
	return;

}


void Garage::displayAllCars()
{

	//cout << "Lane 1:" << setw(10) << "Lane 2:" << setw(10) << "Street:" << endl;
	if (lane1.isEmpty() && lane2.isEmpty() && street.isEmpty())
	{
		cout << "The Garage is empty!" << endl;
		return;
	}

	for (int i = 0; i < 1; i++)
	{
		cout << "Lane 1- ";
		lane1.traverseEntireLane();
		cout << endl;

		cout << "Lane 2- ";
		lane2.traverseEntireLane();
		cout << endl;

		cout << "Street- ";
		street.traverseEntireLane();
		cout << endl << "============================================" << endl << endl << endl;
	}
}