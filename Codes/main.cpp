// Mohammad Asif Zaman
// July 27, 2019
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>
using namespace sf;


const Color dead_cell(26,27,29);
// const Color live_cell(2,252,251);
const Color live_cell(152,121,2);

// const Color fish_color(2,252,251);

const Color fish_color(152,121,2);
const Color shark_color(252,81,81);

const unsigned int cell_size_x = 2;
const unsigned int cell_size_y = 2;



const unsigned int cell_border_thickness = 0;


const unsigned int window_size_x = 800;
const unsigned int window_size_y = 400;
const unsigned int window_extra_y = 220;


const unsigned int offset_x = 0;
const unsigned int offset_y = 5;
const unsigned int plot_offset_x = 50;
const unsigned int plot_offset_y = window_extra_y -20;


const unsigned int Nx = window_size_x/cell_size_x;
const unsigned int Ny = window_size_y/cell_size_y;

const unsigned int fbreed = 3;
const unsigned int sbreed = 6;
const unsigned int starve_max = 4;
const float fish_ratio = 0.01;
const float shark_ratio = 0.005;



unsigned int shark_population[Nx][Ny] = {0};
unsigned int fish_population[Nx][Ny] = {0};
unsigned int shark_starve[Nx][Ny] = {0};



const float update_delay = 0.01;




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



    Text text,text2,text3,text4;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(12);
    text.setPosition(window_size_x - 150,window_size_y + 10);

    text2.setFont(font);
    text2.setString("Press Space to Start/Pause\nPress Esc to quit");
    text2.setCharacterSize(12);
    text2.setPosition(5, window_size_y + offset_y + 1);

    text3.setFont(font);
    text3.setString("Time");
    text3.setCharacterSize(10);
    text3.setPosition(window_size_x/2, window_size_y + window_extra_y -15);

    text4.setFont(font);
    text4.setString("Population");
    text4.setCharacterSize(10);

    text4.setPosition(plot_offset_x-25, window_size_y + window_extra_y*.65);
    text4.setRotation(-90.0);

    RenderWindow window(VideoMode(window_size_x,window_size_y + window_extra_y), "Predetor-Prey dynamics (Wa-Tor)");
    // window.clear(Color(216,27,29));
    window.clear(Color::Black);



    VertexArray grid_draw(Quads,Nx*Ny*4);   // for drawinf fish, shark and empty cells
    VertexArray plot_axes(Lines,4);

    RectangleShape cell(Vector2f(2,2));

    // cell.setFillColor(Color(2,252,251));
    cell.setFillColor(live_cell);
    cell.setOutlineColor(Color(0,0,0));
    cell.setOutlineThickness(cell_border_thickness);


    plot_axes[0].position = Vector2f(plot_offset_x,plot_offset_y + window_size_y);
    plot_axes[1].position = Vector2f(window_size_x-plot_offset_x, plot_offset_y + window_size_y);

    plot_axes[2].position = Vector2f(plot_offset_x,plot_offset_y + window_size_y);
    plot_axes[3].position = Vector2f(plot_offset_x,  window_size_y + 50);

    // plot_axes[4].position = Vector2f(window_size_x-plot_offset_x, plot_offset_y + window_size_y);
    // plot_axes[5].position = Vector2f(window_size_x-plot_offset_x, window_size_y + 50);
    //
    // plot_axes[6].position = Vector2f(window_size_x-plot_offset_x, window_size_y + 50);
    // plot_axes[7].position = Vector2f(plot_offset_x,  window_size_y + 50);
    // plot_axes[2].position = Vector2f(window_size_x, plot_offset_y-300);
    // plot_axes[2].position = Vector2f(plot_offset_x, plot_offset_y-300);








    for (int i = 0; i < Nx; i ++){
      for (int j = 0; j < Ny; j ++){
        float temp = (rand() % 10000)/10000.0;
        // std:: cout << temp << std:: endl;
        if (temp <= fish_ratio) fish_population[i][j] = rand() % fbreed + 1;  // randome interger between 1 and fbreed + 1
        if (temp > fish_ratio && temp < fish_ratio + shark_ratio) shark_population[i][j] = rand() % sbreed + 1;  // randome interger between 1 and sbreed + 1

        if (fish_population[i][j] > 0 && shark_population[i][j] > 0) std:: cout << "Overlap error" << std:: endl;

      }
    }




    unsigned int generation = 1;
    unsigned int n_generation_save = 500;
    unsigned int counter_save = 0;


    bool wait = 1;

    unsigned int fish_count[n_generation_save] = {0};
    unsigned int shark_count[n_generation_save] = {0};


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

      unsigned int x_array[Nx] = {0};
      unsigned int y_array[Ny] = {0};

      for (int i = 0; i < Nx; i ++) x_array[i] = i;
      for (int j = 0; j < Ny; j ++) y_array[j] = j;

      std::random_shuffle(&x_array[0], &x_array[Nx-1]);
      std::random_shuffle(&y_array[0], &y_array[Ny-1]);




      if (clk.getElapsedTime().asSeconds() > update_delay && !wait)
      {
        unsigned int fish_counter = 0;
        unsigned int shark_counter = 0;


        for (int ii = 0; ii < Nx; ii ++){
          for (int jj = 0; jj < Ny; jj ++){
            unsigned int i = x_array[ii];
            unsigned int j = y_array[jj];


            if (shark_population[i][j] > 0 && fish_population[i][j] > 0)  std:: cout << "Fish Shark conflict" << std:: endl;

            if (shark_population[i][j] > 0)  {shark_move(i,j); shark_counter++;}
            if (fish_population[i][j] > 0)  {fish_move(i,j); fish_counter++;}




            // live_cells = live_cells + next_frame[i][j];
          }
        }

        fish_count[counter_save] = fish_counter;
        shark_count[counter_save] = shark_counter;

        counter_save++;
        if (counter_save >= n_generation_save){
          counter_save = n_generation_save-1;
          for (int k = 0; k < n_generation_save - 1; k++) {fish_count[k] = fish_count[k+1]; shark_count[k] = shark_count[k+1];}

        }

        generation = generation + 1;
        oss.str("");
        oss << generation;
        str = "Generation = " + oss.str();
        //
        oss.str("");
        oss << fish_counter;
        str = str + "\nNo. of fish = " + oss.str();

        oss.str("");
        oss << shark_counter;
        str = str + "\nNo. of shark = " + oss.str();

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


      unsigned int counter = 0;
      // Display
      for (int i = 0; i < Nx; i ++){
        for (int j = 0; j < Ny; j ++){

          // cell.setPosition(i*cell_size_x + offset_x, j*cell_size_y + offset_y);


          grid_draw[counter].position = Vector2f(i*cell_size_x + offset_x, j*cell_size_y + offset_y);
          grid_draw[counter+1].position = Vector2f((i+1)*cell_size_x + offset_x, j*cell_size_y + offset_y);
          grid_draw[counter+2].position = Vector2f((i+1)*cell_size_x + offset_x, (j+1)*cell_size_y + offset_y);
          grid_draw[counter+3].position = Vector2f(i*cell_size_x + offset_x, (j+1)*cell_size_y + offset_y);




          if (fish_population[i][j] > 0){
            cell.setFillColor(fish_color);
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






    }  // window.isOpen
    // std:: cout << generation;
    return 0;
}






// User defined functions





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
