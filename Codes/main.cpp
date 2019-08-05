// Mohammad Asif Zaman
// July-August, 2019
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>
using namespace sf;



// Define colors
const Color dead_cell(26,27,29);
const Color fish_color(2,252,251);
const Color shark_color(252,81,81);

// const Color fish_color(152,121,2);
// const Color fish_color(190,190,20);


// Defining window size
const unsigned int window_size_x = 700;  // x size of the grid in pixels
const unsigned int window_size_y = 400;  // y size of the grid in pixels
const unsigned int window_extra_y = 220;  // Extra y space after the grid for text and plots


// Define the size of each cell in pixels
const unsigned int cell_size_x = 2;
const unsigned int cell_size_y = 2;
const unsigned int cell_border_thickness = 0;

// Calculate number of cells based on the window size and cell size
const unsigned int Nx = window_size_x/cell_size_x;
const unsigned int Ny = window_size_y/cell_size_y;


// Window offset and other parameters for display
const unsigned int offset_x = 0;
const unsigned int offset_y = 5;
const unsigned int plot_offset_x = 50;
const unsigned int plot_offset_y = window_extra_y -20;


// Predetor-prey dynimics parameters
const unsigned int fbreed = 3;   // fish breed time
const unsigned int sbreed = 6;   // shark breed time
const unsigned int starve_max = 4;  // shark maximum starve time

// Ratio for initial population
const float fish_ratio = 0.01;
const float shark_ratio = 0.005;


// Arrays for storing fish/shark age and shark starve value
unsigned int shark_population[Nx][Ny] = {0};
unsigned int fish_population[Nx][Ny] = {0};
unsigned int shark_starve[Nx][Ny] = {0};


// Display update delay. Frame rate should be the inverse of this
const float update_delay = 0.01;




// Main function

int main()
{

// function prototype
    void shark_move(int i, int j);
    void fish_move(int i, int j);


// Random seed and start clock
    srand(time(0));
    Clock clk;
    clk.restart();    // starting a clock

// Variables
    bool wait = 1;                                        // Pause variable
    unsigned int generation = 1;                          // counting generations
    unsigned int n_generation_save = 500;                 // Number of generations to save and display in the plot
    unsigned int counter_save = 0;                        // Counting how many generation data has been saved
    unsigned int fish_count[n_generation_save] = {0};     // Array to store fish count at each generation
    unsigned int shark_count[n_generation_save] = {0};    // Array to store shark count at each generation





// Loading fonts
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")){
        std::cout << "Error loading font from file" << std::endl;
        system("pause");
    }


    std::stringstream oss;
    oss << 1;
    std::string str = "Generation = " + oss.str();


    // Defining texts
    Text text,text2,text3,text4;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(12);
    text.setPosition(window_size_x*.75, window_size_y + 10);

    text2.setFont(font);
    text2.setString("Press Space to Start/Pause\nPress Esc to quit");
    text2.setCharacterSize(12);
    text2.setPosition(15, window_size_y + offset_y + 1);

    text3.setFont(font);
    text3.setString("Time");
    text3.setCharacterSize(10);
    text3.setPosition(window_size_x/2, window_size_y + window_extra_y -15);

    text4.setFont(font);
    text4.setString("Fish Population,\nShark Population x 4");
    text4.setCharacterSize(10);

    text4.setPosition(plot_offset_x-35, window_size_y + window_extra_y*.7);
    text4.setRotation(-90.0);




// Setting up drawing entities. VertexArrays and Rectangle shapes
    VertexArray grid_draw(Quads,Nx*Ny*4);   // for drawing fish, shark and empty cells
    VertexArray plot_axes(Lines,4);         // for drawing plot axes

    RectangleShape cell(Vector2f(2,2));              // for drawing the plot
    RectangleShape cell_legend_fish(Vector2f(5,5));  // for drawing the fish legend on the plot
    RectangleShape cell_legend_shark(Vector2f(5,5)); // for drawing the shark legend on the plot

    // cell.setFillColor(Color(2,252,251));

// Initializing
    cell.setFillColor(fish_color);
    cell.setOutlineColor(Color(0,0,0));
    cell.setOutlineThickness(cell_border_thickness);

// Setting the position, color etc. of the plot legends
    cell_legend_fish.setFillColor(fish_color);
    cell_legend_fish.setOutlineColor(Color(0,0,0));
    cell_legend_fish.setOutlineThickness(cell_border_thickness);
    cell_legend_fish.setPosition(window_size_x*0.75-12,window_size_y + window_extra_y*.27);

    cell_legend_shark.setFillColor(shark_color);
    cell_legend_shark.setOutlineColor(Color(0,0,0));
    cell_legend_shark.setOutlineThickness(cell_border_thickness);
    cell_legend_shark.setPosition(window_size_x*0.75-12,window_size_y + window_extra_y*.27+13);

    plot_axes[0].position = Vector2f(plot_offset_x,plot_offset_y + window_size_y);
    plot_axes[1].position = Vector2f(window_size_x-plot_offset_x, plot_offset_y + window_size_y);
    plot_axes[2].position = Vector2f(plot_offset_x,plot_offset_y + window_size_y);
    plot_axes[3].position = Vector2f(plot_offset_x,  window_size_y + 50);



// Render the window
    RenderWindow window(VideoMode(window_size_x,window_size_y + window_extra_y), "Predetor-Prey dynamics (Wa-Tor)");
    // window.clear(Color(216,27,29));
    window.clear(Color::Black);



// Initial populizing the grid
    for (int i = 0; i < Nx; i ++){
      for (int j = 0; j < Ny; j ++){
        float temp = (rand() % 10000)/10000.0;  // Get a randome number between 0 and 1 with 4 decimal places
        // std:: cout << temp << std:: endl;

        // populate grid based on the initial population ratio defined. Also, the cells are initialized with random age value between 1 and breeding age
        if (temp <= fish_ratio) fish_population[i][j] = rand() % fbreed + 1;  // randome interger between 1 and fbreed
        if (temp > fish_ratio && temp < fish_ratio + shark_ratio) shark_population[i][j] = rand() % sbreed + 1;  // randome interger between 1 and sbreed

        // Check for overlap error
        if (fish_population[i][j] > 0 && shark_population[i][j] > 0) std:: cout << "Overlap error" << std:: endl;

      }
    }






    while(window.isOpen())
    {

  // Variables
      Event e;                            // Event variable
      unsigned int x_array[Nx] = {0};     // Array with order index
      unsigned int y_array[Ny] = {0};     // Array with order index

      // Filling with sequential indices
      for (int i = 0; i < Nx; i ++) x_array[i] = i;
      for (int j = 0; j < Ny; j ++) y_array[j] = j;

      // Randomizing order indices
      std::random_shuffle(&x_array[0], &x_array[Nx-1]);
      std::random_shuffle(&y_array[0], &y_array[Ny-1]);



      // Check for keyboard presses
      while (window.pollEvent(e))
      {
        if (e.type == Event::Closed || e.key.code == Keyboard::Escape)   window.close();
        if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space)  wait = !wait;
      }



// Update
// Calculate the new location of the fishes and sharks

      if (clk.getElapsedTime().asSeconds() > update_delay && !wait)
      {
        // Counters for counting fishes and sharks
        unsigned int fish_counter = 0;
        unsigned int shark_counter = 0;

        // Nested loops to go through each grid point
        for (int ii = 0; ii < Nx; ii ++){
          for (int jj = 0; jj < Ny; jj ++){
            unsigned int i = x_array[ii];
            unsigned int j = y_array[jj];

            // Check for conflicts (again)
            if (shark_population[i][j] > 0 && fish_population[i][j] > 0)  std:: cout << "Fish Shark conflict" << std:: endl;

            // Call move functions only if the cell has a fish or a shark in it. Also count the fishes and sharks.
            if (shark_population[i][j] > 0)  {shark_move(i,j); shark_counter++;}
            if (fish_population[i][j] > 0)  {fish_move(i,j); fish_counter++;}

          }
        }


        // Save fish and shark counts for each generation
        fish_count[counter_save] = fish_counter;
        shark_count[counter_save] = shark_counter;

        counter_save++;   // increment save counter

        // If counter_save is >= than the number of saved values we want, then left shift array by 1 (eliminating the first entry and making room at the last entry location)
        if (counter_save >= n_generation_save){
          counter_save = n_generation_save-1;   // Decrease counter save so that the last element is indexed
          for (int k = 0; k < n_generation_save - 1; k++) {fish_count[k] = fish_count[k+1]; shark_count[k] = shark_count[k+1];}  // left shift the array
        }

        generation = generation + 1;   // increment generation Number


        // Display texts
        oss.str("");
        oss << generation;
        str = "Generation = " + oss.str();

        oss.str("");
        oss << fish_counter;
        str = str + "\n\n\nFish population = " + oss.str();

        oss.str("");
        oss << shark_counter;
        str = str + "\nShark population = " + oss.str();


        text.setString(str);


        clk.restart();   // Restart the clock (very important)
      }



      // Display
      unsigned int counter = 0;
      for (int i = 0; i < Nx; i ++){
        for (int j = 0; j < Ny; j ++){

          // cell.setPosition(i*cell_size_x + offset_x, j*cell_size_y + offset_y);


          grid_draw[counter].position = Vector2f(i*cell_size_x + offset_x, j*cell_size_y + offset_y);
          grid_draw[counter+1].position = Vector2f((i+1)*cell_size_x + offset_x, j*cell_size_y + offset_y);
          grid_draw[counter+2].position = Vector2f((i+1)*cell_size_x + offset_x, (j+1)*cell_size_y + offset_y);
          grid_draw[counter+3].position = Vector2f(i*cell_size_x + offset_x, (j+1)*cell_size_y + offset_y);




          if (fish_population[i][j] > 0){
            // cell.setFillColor(fish_color);
            grid_draw[counter].color = fish_color;
            grid_draw[counter+1].color = fish_color;
            grid_draw[counter+2].color = fish_color;
            grid_draw[counter+3].color = fish_color;
          } else{
            // cell.setFillColor(dead_cell);
            grid_draw[counter].color = dead_cell;
            grid_draw[counter+1].color = dead_cell;
            grid_draw[counter+2].color = dead_cell;
            grid_draw[counter+3].color = dead_cell;
          }





          if (shark_population[i][j] > 0){

            grid_draw[counter].color = shark_color;
            grid_draw[counter+1].color = shark_color;
            grid_draw[counter+2].color = shark_color;
            grid_draw[counter+3].color = shark_color;
          }

          // window.draw(cell);
          // int temp1 = rand()%3;
          // if (temp1 == 2)  std:: cout << temp1 << std:: endl;
          counter = counter + 4;

        }   // end j
        // window.draw(cell);
      }   // end i

      window.draw(grid_draw);
      window.draw(plot_axes);

      // Graph plotting
      for (int k = 0; k < counter_save; k++){
          cell.setPosition((window_size_x-2*plot_offset_x)*k/n_generation_save + plot_offset_x, window_size_y + plot_offset_y  -  150*fish_count[k]/(Nx*Ny));
          cell.setFillColor(fish_color);
          window.draw(cell);
          cell.setPosition((window_size_x-2*plot_offset_x)*k/n_generation_save  + plot_offset_x, window_size_y + plot_offset_y -  600*shark_count[k]/(Nx*Ny));
          cell.setFillColor(shark_color);
          window.draw(cell);
      }



      window.display();
      window.clear(Color::Black);

      window.draw(text);
      window.draw(text2);
      window.draw(text3);
      window.draw(text4);
      window.draw(cell_legend_fish);
      window.draw(cell_legend_shark);


    }  // window.isOpen
    // std:: cout << generation;
    return 0;
}













//////////////////////////////////////////////////////////////////////////////
// User defined functions
/////////////////////////////////////////////////////////////////////////////




void fish_move(int ix, int jy)
{

      // Arrays to save empty neigboring cell locations
      int ix_free[8] = {0};
      int jy_free[8] = {0};



      int counter_free = 0;  // counter to count free neighboring cells

      int x_ind = 0;
      int y_ind = 0;

      unsigned int old_pos_age = 0;
      unsigned int new_pos_age = 0;


      // aging + breeding
      if (fish_population[ix][jy] >= fbreed){
        old_pos_age = 1;
        new_pos_age = 1;
      } else{
        old_pos_age = 0;
        new_pos_age = fish_population[ix][jy] + 1;
      }




      // Nested loops to go through all the neighboring cells
      for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++ ){

          // if condition to avoid counting its ownself
          if (i*i + j*j != 0)
          {

            x_ind = ix + i;
            y_ind = jy + j;


            // Toroidal boundary condition
            if (x_ind < 0) x_ind = Nx-1;
            if (x_ind  >= Nx) x_ind = 0;
            if (y_ind < 0) y_ind = Ny-1;
            if (y_ind >= Ny) y_ind = 0;


            // record position of empty cell
            if (fish_population[x_ind][y_ind] == 0 && shark_population[x_ind][y_ind] == 0){
              ix_free[counter_free] = x_ind;
              jy_free[counter_free] = y_ind;
              counter_free++;
            }



          } // end if i!=0 && j!=0


        }  // end for j
      } // end for i




      // if the fish finds an empty neigboring cell
      if (counter_free != 0){
        // std:: cout << "empty cell movement" << std:: endl;
        int temp1 = rand() % counter_free;

        unsigned int iix = ix_free[temp1];
        unsigned int jjy = jy_free[temp1];


        fish_population[iix][jjy] = new_pos_age;  // move shark to new position with updated age
        fish_population[ix][jy] = old_pos_age;   // update age at the previous position of the shark (0 for empty cell, 1 for new shark)
      } else {  // fish does not find an empty neighboring cell

        fish_population[ix][jy] = new_pos_age;


      }




}






void shark_move(int ix, int jy)
{


      // Arrays to save fish neighbors locations
      int ix_neighbor[8] = {0};
      int jy_neighbor[8] = {0};

      // Arrays to save empty neigboring cell locations
      int ix_free[8] = {0};
      int jy_free[8] = {0};


      int counter = 0;       // counter to count fish neighbors
      int counter_free = 0;  // counter to count free neighboring cells

      int x_ind = 0;
      int y_ind = 0;

      unsigned int old_pos_age = 0;
      unsigned int new_pos_age = 0;


      // aging + breeding
      if (shark_population[ix][jy] >= sbreed){
        old_pos_age = 1;
        new_pos_age = 1;
      } else{
        old_pos_age = 0;
        new_pos_age = shark_population[ix][jy] + 1;
      }




      // Nested loops to go through all the neighboring cells
      for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++ ){

          // if condition to avoid counting its ownself
          if (i*i + j*j != 0)
          {

            x_ind = ix + i;
            y_ind = jy + j;


            // Toroidal boundary condition
            if (x_ind < 0) x_ind = Nx-1;
            if (x_ind  >= Nx) x_ind = 0;
            if (y_ind < 0) y_ind = Ny-1;
            if (y_ind >= Ny) y_ind = 0;

            // record the cell if it has fish in it
            if (fish_population[x_ind][y_ind] > 0){
              ix_neighbor[counter] = x_ind;
              jy_neighbor[counter] = y_ind;
              counter++;
            }

            // record position of empty cell
            if (fish_population[x_ind][y_ind] == 0 && shark_population[x_ind][y_ind] == 0){
              ix_free[counter_free] = x_ind;
              jy_free[counter_free] = y_ind;
              counter_free++;
            }



          } // end if i!=0 && j!=0


        }  // end for j
      } // end for i



      // if the shark finds a neighboring fish
      if (counter != 0){
        int temp1 = rand() % counter;

        // std:: cout << "found fish" << std:: endl;

        unsigned int iix = ix_neighbor[temp1];
        unsigned int jjy = jy_neighbor[temp1];

        fish_population[iix][jjy] = 0;   // Remove fish from the current position of the shark (eaten)

        shark_population[iix][jjy] = new_pos_age;  // move shark to new position and update age
        shark_population[ix][jy] = old_pos_age;   // update previous position of the shark (either new shark or empty)


        shark_starve[ix][jy] = 0; // reset starve value of empty/new shark cell
        shark_starve[iix][jjy] = 0;   // as shark has eaten, current starve value is 0.

      }




      // if the shark does not find a fish but finds an empty neigboring cell
      if (counter == 0 && counter_free != 0){
        // std:: cout << "empty cell movement" << std:: endl;
        int temp1 = rand() % counter_free;

        unsigned int iix = ix_free[temp1];
        unsigned int jjy = jy_free[temp1];

        // std:: cout << counter_free << ',' << temp1 << std:: endl;
        // std:: cout << ix << ',' << jy << "," << ix_free[temp1] << "," << jy_free[temp1] <<  std:: endl;
        shark_population[iix][jjy] = new_pos_age;  // move shark to new position with updated age
        shark_population[ix][jy] = old_pos_age;   // update age at the previous position of the shark (0 for empty cell, 1 for new shark)
        shark_starve[iix][jjy] = shark_starve[ix][jy] + 1; // update starve of current cell as increment over previous value
        shark_starve[ix][jy] = 0;    // ths cell is now empty, so it has no starve value.

        // std:: cout << shark_starve[iix][jjy] << std:: endl;

        // check if starve value > starve_max. If so, then kill shark (empty cell and reset starve value of empty cell)
        if (shark_starve[iix][jjy] >= starve_max){
          shark_population[iix][jjy] = 0;
          shark_starve[iix][jjy] = 0;
          shark_population[ix][jy] = 0;  // reset breeding (if it starved, it didn't breed.)
        }
      }


      // if the shark can't find a fish and has no empty neighboring cells
      if (counter == 0 && counter_free == 0){
        // std:: cout << "no movement" << std:: endl;
        shark_population[ix][jy] = new_pos_age;

        shark_starve[ix][jy] = shark_starve[ix][jy] + 1; // increase starve value of empty cells
        // std:: cout <<  shark_starve[ix][jy] << std:: endl;
        // check if starve value > starve_max. If so, then kill shark (empty cell and reset starve value of empty cell)
        if (shark_starve[ix][jy] >= starve_max){
          // std:: cout << "no movement death" << std:: endl;
          shark_population[ix][jy] = 0;  // shark is dead
          shark_starve[ix][jy] = 0; // reset starve value
        }

      }

      // Note that if none of the above 2 if conditions are satisfied, the global veriables of the shark population and the fish population are not
      // updated. This is equivalent to the shark not moving.



}
