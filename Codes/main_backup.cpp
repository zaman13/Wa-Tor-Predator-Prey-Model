// Mohammad Asif Zaman
// July 27, 2019
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;


const Color dead_cell(26,27,29);
// const Color live_cell(2,252,251);
const Color live_cell(152,121,2);

const Color fish_color(2,252,251);
const Color shark_color(252,81,81);

const unsigned int cell_size_x = 5;
const unsigned int cell_size_y = 5;

const unsigned int offset_x = 0;
const unsigned int offset_y = 5;

const unsigned int cell_border_thickness = 1;


const unsigned int window_size_x = 600;
const unsigned int window_size_y = 600;
const unsigned int window_extra_y = 60;

const unsigned int Nx = window_size_x/cell_size_x;
const unsigned int Ny = window_size_x/cell_size_x;

const unsigned int fbreed = 4;
const unsigned int sbreed = 10;
const unsigned int starve_max = 8;
const float fish_ratio = 0.03;
const float shark_ratio = 0.02;



unsigned int shark_population[Nx][Ny] = {0};
unsigned int fish_population[Nx][Ny] = {0};
unsigned int shark_starve[Nx][Ny] = {0};


const float update_delay = 0.002;




int main()
{


    // function prototype
    int neighbor_counter(int arg1, int arg2, unsigned int arg3[Nx][Ny]);
    void shark_move(int i, int j);
    void fish_move(int i, int j);


    srand(time(0));
    Clock clk;
    clk.restart();    // starting a clock


    // unsigned int random = rand() % 2;

    Font font;

    if (!font.loadFromFile("Fonts/arial.ttf")){
        std::cout << "Error loading font from file" << std::endl;
        system("pause");
    }


    std::stringstream oss;
    oss << 1;
    std::string str = "Generation = " + oss.str();



    Text text,text2;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(12);
    text.setPosition(window_size_x - 150,window_size_y + 10);

    text2.setFont(font);
    text2.setString("Press Space to Start/Pause\nClick mouse while paused to toggle a cell\nPress Esc to quit");
    text2.setCharacterSize(12);
    text2.setPosition(5, window_size_y + offset_y + 1);




    RenderWindow window(VideoMode(window_size_x,window_size_y + window_extra_y), "Predetor-Prey dynamics (Wa-Tor)");
    // window.clear(Color(216,27,29));
    window.clear(Color::Black);

    RectangleShape cell(Vector2f(cell_size_x,cell_size_y));
    // cell.setFillColor(Color(2,252,251));
    cell.setFillColor(live_cell);
    cell.setOutlineColor(Color(0,0,0));
    cell.setOutlineThickness(cell_border_thickness);




    unsigned int population_frame[Nx][Ny];
    unsigned int next_frame[Nx][Ny];

    for (int i = 0; i < Nx; i ++){
      for (int j = 0; j < Ny; j ++){
        float temp = (rand() % 100)/100.0;
        // std:: cout << temp << std:: endl;
        if (temp <= fish_ratio) fish_population[i][j] = rand() % fbreed + 1;  // randome interger between 1 and fbreed + 1
        if (temp > fish_ratio && temp < fish_ratio + shark_ratio) shark_population[i][j] = rand() % sbreed + 1;  // randome interger between 1 and sbreed + 1

        if (fish_population[i][j] > 0 && shark_population[i][j] > 0) std:: cout << "Overlap error" << std:: endl;
        population_frame[i][j] = rand() % 2;
      }
    }




    unsigned int generation = 1;

    bool wait = 1;

    while(window.isOpen())
    {


      Event e;




      while (window.pollEvent(e))
      {
        if (e.type == Event::Closed || e.key.code == Keyboard::Escape)
          window.close();

        if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space)
          wait = !wait;


      }



      // Update
      // replace population_frame with next_frame

      if (clk.getElapsedTime().asSeconds() > update_delay && !wait)
      {
        unsigned int live_cells = 0;
        for (int i = 0; i < Nx; i ++){
          for (int j = 0; j < Ny; j ++){

            if (shark_population[i][j] > 0 && fish_population[i][j] > 0)  std:: cout << "Fish Shark conflict" << std:: endl;

            if (shark_population[i][j] > 0)  shark_move(i,j);
            if (fish_population[i][j] > 0)  fish_move(i,j);




            // live_cells = live_cells + next_frame[i][j];
          }
        }

        generation = generation + 1;
        oss.str("");
        oss << generation;
        str = "Generation = " + oss.str();
        //
        // oss.str("");
        // oss << live_cells;
        // str = str + "\nLive cells = " + oss.str();
        //
        //
        // oss.str("");
        // int temp = live_cells*10000/(Nx*Ny);
        // float percent_live = temp/100.0;
        // oss << percent_live;
        // str = str + " (" + oss.str() + "%)";
        //
        //
        //
        //
        text.setString(str);
        // text.setPosition(10,window_size_y + 5);


        clk.restart();
      }



      // Display
      for (int i = 0; i < Nx; i ++){
        for (int j = 0; j < Ny; j ++){

          cell.setPosition(i*cell_size_x + offset_x, j*cell_size_y + offset_y);

          if (fish_population[i][j] > 0){
            cell.setFillColor(fish_color);
          } else{
            cell.setFillColor(dead_cell);
          }





          if (shark_population[i][j] > 0){
            cell.setFillColor(shark_color);
          }

          window.draw(cell);
          // int temp1 = rand()%3;
          // if (temp1 == 2)  std:: cout << temp1 << std:: endl;


        }
        // window.draw(cell);
      }



      // Calculate the next frame according to the rules (based on neighbor count)

      // for (int i = 0; i < Nx; i ++){
      //   for (int j = 0; j < Ny; j ++){
      //
      //     cell.setPosition(i*cell_size_x + offset_x, j*cell_size_y + offset_y);
      //
      //     if (population_frame[i][j] == 1){
      //       cell.setFillColor(live_cell);
      //     } else{
      //       cell.setFillColor(dead_cell);
      //     }
      //
      //     window.draw(cell);
      //
      //
      //     // count the number of neighbors
      //
      //     int neighbor_n = 0;
      //
      //     neighbor_n = neighbor_counter(i,j,population_frame);
      //
      //
      //     next_frame[i][j] = population_frame[i][j];
      //
      //     if (population_frame[i][j] == 0){
      //         if (neighbor_n == 3)   next_frame[i][j] = 1;
      //     }
      //
      //
      //     if (population_frame[i][j] == 1){
      //         if ( (neighbor_n < 2) | (neighbor_n > 3))  next_frame[i][j] = 0;
      //     }
      //
      //
      //
      //
      //   }   // for j
      // } // for i
      //



      window.display();
      window.clear(Color::Black);

      window.draw(text);
      window.draw(text2);






    }  // window.isOpen
    // std:: cout << generation;
    return 0;
}






// User defined functions


// Function for counting neighbors

int neighbor_counter(int ix,int jy, unsigned int population[Nx][Ny])
{
    int count = 0;

    for (int i = -1; i < 2; i ++){
      for (int j = -1; j < 2; j++ ){
        if (ix+i >= -1 && ix+i < Nx & jy+j >= -1 && jy+j < Ny ){
          count += population[ix+i][jy+j];
        }
      }
    }

    return count - population[ix][jy];

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
