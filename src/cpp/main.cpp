#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include "assert.h"

//SFML - A library for Drawing & Rendering Objects - The UI was built on top of this
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "include/BasisFunc.h"
#include "include/Matrix.h"
#include "include/extra_functions.h"

bool isMouseOver_Rectangle(sf::ConvexShape *query, sf::RenderWindow *window);
bool isMouseOver_Circle(sf::CircleShape *query, sf::RenderWindow *window);

using namespace std;

// All UI elements and behaviours were designed by me - Apart from the SFML includes (& the Vector class, which is a modification of a class taken from a module in my university), all classes, methods and behaviours in
// this project were also written by me as either part of my dissertation or (in specifically the UI & web integration) for my project portfolio at adam-howlett.github.io/dissertation.html

int main(){

    //Creating Main Window
    sf::RenderWindow window(sf::VideoMode(750, 810), "DDMRes Calculator", sf::Style::Close);
    window.setFramerateLimit(60);

    //sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;

    //Loading in Textures/Fonts
    sf::Font calibri;
    sf::Font calibri_bold;
    sf::Texture border_texture;

    sf::Texture rectangular_button_unselected;
    sf::Texture rectangular_button_hover;
    sf::Texture rectangular_button_selected;

    sf::Texture button_selected;
    sf::Texture button_unselected;

    sf::Texture sliderbar_angle;
    sf::Texture slider_button;
    sf::Texture angle_arrow_texture;
    sf::Texture graph_texture;

    sf::Texture sliderbar_resolution;

    sf::Texture unrefined_1;
    sf::Texture redrefined_1;
    sf::Texture verticalrefined_1;

    sf::Texture unrefined_2;
    sf::Texture redrefined_2;
    sf::Texture verticalrefined_2;

    sf::Texture unrefined_4;
    sf::Texture redrefined_4;
    sf::Texture verticalrefined_4;

    sf::Texture unrefined_8;
    sf::Texture redrefined_8;
    sf::Texture verticalrefined_8;

    sf::Texture unrefined_16;
    sf::Texture redrefined_16;
    sf::Texture verticalrefined_16;

    sf::Texture function1_im;

    {
        calibri.loadFromFile("resources/calibri.ttf");
        calibri_bold.loadFromFile("resources/calibrib.ttf");
        border_texture.loadFromFile("resources/border.png");
        button_selected.loadFromFile("resources/button_selected.png");
        button_unselected.loadFromFile("resources/button_unselected.png");
        rectangular_button_unselected.loadFromFile("resources/rectangular_button.png");
        rectangular_button_hover.loadFromFile("resources/rectangular_button_hover.png");
        rectangular_button_selected.loadFromFile("resources/rectangular_button_selected.png");
        sliderbar_angle.loadFromFile("resources/sliderbar_selected.png");
        slider_button.loadFromFile("resources/slider_button.png");
        angle_arrow_texture.loadFromFile("resources/angle_arrow.png");
        graph_texture.loadFromFile("resources/graph.png");
        sliderbar_resolution.loadFromFile("resources/sliderbar_resolution.png");

        unrefined_1.loadFromFile("resources/1_unrefined.png");
        unrefined_2.loadFromFile("resources/2_unrefined.png");
        unrefined_4.loadFromFile("resources/4_unrefined.png");
        unrefined_8.loadFromFile("resources/8_unrefined.png");
        unrefined_16.loadFromFile("resources/16_unrefined.png");
        redrefined_1.loadFromFile("resources/1_redrefined.png");
        redrefined_2.loadFromFile("resources/2_redrefined.png");
        redrefined_4.loadFromFile("resources/4_redrefined.png");
        redrefined_8.loadFromFile("resources/8_redrefined.png");
        redrefined_16.loadFromFile("resources/16_redrefined.png");
        verticalrefined_1.loadFromFile("resources/1_verticalrefined.png");
        verticalrefined_2.loadFromFile("resources/2_verticalrefined.png");
        verticalrefined_4.loadFromFile("resources/4_verticalrefined.png");
        verticalrefined_8.loadFromFile("resources/8_verticalrefined.png");
        verticalrefined_16.loadFromFile("resources/16_verticalrefined.png");

    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Setting up shapes and positions for UI (Using SFML for a UI was mainly for the sake of an interesting coding challenge in terms of designing elements and behaviours from the ground up, i'm fully aware there were much quicker options for this) //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////
    //Border, Background & Title//
    //////////////////////////////
    sf::ConvexShape border;
    border.setPointCount(4);
    border.setPoint(0, sf::Vector2f(0,0));
    border.setPoint(1, sf::Vector2f(0,810));
    border.setPoint(2, sf::Vector2f(750,810));
    border.setPoint(3, sf::Vector2f(750,0));

    sf::ConvexShape background = border;
    background.setPoint(0, sf::Vector2f(1,1));
    background.setPoint(1, sf::Vector2f(1,819));
    background.setPoint(2, sf::Vector2f(749,819));
    background.setPoint(3, sf::Vector2f(749,1));
    background.setFillColor(sf::Color::White);
    border.setTexture(&border_texture);

    sf::Text title_text;
    title_text.setFont(calibri_bold);
    title_text.setCharacterSize(24);
    title_text.setFillColor(sf::Color::Black);
    title_text.setPosition(sf::Vector2f(40,36));
    title_text.setString("DDMRes Solver & Plotter");


    /////////////////////
    //Matlab Selection //
    /////////////////////
    sf::Text matlab_text;
    matlab_text.setFont(calibri_bold);
    matlab_text.setCharacterSize(18);
    matlab_text.setFillColor(sf::Color::Black);
    matlab_text.setPosition(sf::Vector2f(40,70));
    matlab_text.setString("Do you have MATLAB installed?");

    sf::Text matlab_text_2 = matlab_text;
    matlab_text_2.setFont(calibri);
    matlab_text_2.setPosition(sf::Vector2f(40,91));
    matlab_text_2.setString("Yes:         No:");

    sf::Text matlab_ifno_text;
    matlab_ifno_text.setFont(calibri);
    matlab_ifno_text.setCharacterSize(12);
    matlab_ifno_text.setFillColor(sf::Color::Red);
    matlab_ifno_text.setPosition(sf::Vector2f(165,96));
    matlab_ifno_text.setString("Solutions saved in \"bin\\Solution.txt\"");

    sf::CircleShape matlab_yes_select(8);
    matlab_yes_select.setTexture(&button_selected);
    matlab_yes_select.setPosition(sf::Vector2f(75,96));
    bool matlab_installed = true;
    bool matlab_running = false;

    sf::CircleShape matlab_no_select(8);
    matlab_no_select.setTexture(&button_unselected);
    matlab_no_select.setPosition(sf::Vector2f(140,96));



    //////////////////////
    //Function Selection//
    //////////////////////

    int selection = 1;
    sf::Text function_select_text;
    function_select_text.setFont(calibri_bold);
    function_select_text.setCharacterSize(18);
    function_select_text.setFillColor(sf::Color::Black);
    function_select_text.setPosition(sf::Vector2f(40,120));
    function_select_text.setString("Function to Approximate");

    sf::ConvexShape function1_button;
    function1_button.setPointCount(4);
    function1_button.setPoint(0,sf::Vector2f(80,160));
    function1_button.setPoint(1,sf::Vector2f(200,160));
    function1_button.setPoint(2,sf::Vector2f(200,190));
    function1_button.setPoint(3,sf::Vector2f(80,190));
    function1_button.setTexture(&rectangular_button_selected);

    sf::Text function1_text = function_select_text;
    function1_text.setFont(calibri);
    function1_text.setCharacterSize(16);
    function1_text.setPosition(sf::Vector2f(115,165));
    function1_text.setString("sin(pi*x)");


    sf::ConvexShape function2_button = function1_button;
    function2_button.setPoint(0,sf::Vector2f(220,160));
    function2_button.setPoint(1,sf::Vector2f(340,160));
    function2_button.setPoint(2,sf::Vector2f(340,190));
    function2_button.setPoint(3,sf::Vector2f(220,190));
    function2_button.setTexture(&rectangular_button_unselected);

    sf::Text function2_text = function1_text;
    function2_text.setPosition(sf::Vector2f(240,165));
    function2_text.setString("e^x(1-cos(y))");


    sf::ConvexShape function3_button = function2_button;
    function3_button.setPoint(0,sf::Vector2f(80,210));
    function3_button.setPoint(1,sf::Vector2f(200,210));
    function3_button.setPoint(2,sf::Vector2f(200,240));
    function3_button.setPoint(3,sf::Vector2f(80,240));

    sf::Text function3_text = function1_text;
    function3_text.setCharacterSize(16);
    function3_text.setPosition(sf::Vector2f(105,215));
    function3_text.setString("cos(pi*x*y)");


    sf::ConvexShape function4_button = function2_button;
    function4_button.setPoint(0,sf::Vector2f(220,210));
    function4_button.setPoint(1,sf::Vector2f(340,210));
    function4_button.setPoint(2,sf::Vector2f(340,240));
    function4_button.setPoint(3,sf::Vector2f(220,240));

    sf::Text function4_text = function1_text;
    function4_text.setPosition(sf::Vector2f(232,215));
    function4_text.setString(" e^x cos(pi*y)");




    /////////////////////
    //Refinement Scheme//
    /////////////////////

    int refinement_scheme = 1;

    sf::Text refinement_select_text;
    refinement_select_text.setFont(calibri_bold);
    refinement_select_text.setCharacterSize(18);
    refinement_select_text.setFillColor(sf::Color::Black);
    refinement_select_text.setPosition(sf::Vector2f(40,260));
    refinement_select_text.setString("Refinement Scheme");

    sf::ConvexShape redrefine_button;
    redrefine_button.setPointCount(4);
    redrefine_button.setPoint(0,sf::Vector2f(80,300));
    redrefine_button.setPoint(1,sf::Vector2f(200,300));
    redrefine_button.setPoint(2,sf::Vector2f(200,330));
    redrefine_button.setPoint(3,sf::Vector2f(80,330));
    redrefine_button.setTexture(&rectangular_button_selected);

    sf::Text redrefine_text = function_select_text;
    redrefine_text.setFont(calibri);
    redrefine_text.setCharacterSize(16);
    redrefine_text.setPosition(sf::Vector2f(120,305));
    redrefine_text.setString("\"Red\"");


    sf::ConvexShape linerefine_button = redrefine_button;
    linerefine_button.setPoint(0,sf::Vector2f(220,300));
    linerefine_button.setPoint(1,sf::Vector2f(340,300));
    linerefine_button.setPoint(2,sf::Vector2f(340,330));
    linerefine_button.setPoint(3,sf::Vector2f(220,330));
    linerefine_button.setTexture(&rectangular_button_unselected);

    sf::Text linerefine_text = redrefine_text;
    linerefine_text.setPosition(sf::Vector2f(240,305));
    linerefine_text.setString("Vertical Line");

    sf::ConvexShape WhiteOut_VerticalLine = linerefine_button;
    WhiteOut_VerticalLine.setTexture(NULL);
    WhiteOut_VerticalLine.setFillColor(sf::Color(255,255,255,100));

    ////////////////
    //Angle Slider//
    ////////////////


    bool isGrabbed_angle_button = false;
    double angle;

    sf::Text angle_slider_text;
    angle_slider_text.setFont(calibri_bold);
    angle_slider_text.setCharacterSize(18);
    angle_slider_text.setFillColor(sf::Color::Black);
    angle_slider_text.setPosition(sf::Vector2f(40,345));
    angle_slider_text.setString("Velocity Field Angle");

    sf::ConvexShape angle_slider_bar;
    angle_slider_bar.setPointCount(4);
    angle_slider_bar.setPoint(0,sf::Vector2f(40,365));
    angle_slider_bar.setPoint(1,sf::Vector2f(360,365));
    angle_slider_bar.setPoint(2,sf::Vector2f(360,410));
    angle_slider_bar.setPoint(3,sf::Vector2f(40,410));
    angle_slider_bar.setTexture(&sliderbar_angle);

    sf::CircleShape angle_slider_button(8);
    angle_slider_button.setTexture(&slider_button);
    angle_slider_button.setPosition(sf::Vector2f(273,379));


    sf::Text angle_slider_angle_text;
    angle_slider_angle_text.setFont(calibri);
    angle_slider_angle_text.setFillColor(sf::Color::Black);
    angle_slider_angle_text.setCharacterSize(18);
    angle_slider_angle_text.setPosition(180,405);
    angle_slider_angle_text.setString("0");


    sf::ConvexShape angle_graph;
    angle_graph.setPointCount(4);
    angle_graph.setPoint(0,sf::Vector2f(50,410));
    angle_graph.setPoint(1,sf::Vector2f(170,410));
    angle_graph.setPoint(2,sf::Vector2f(170,530));
    angle_graph.setPoint(3,sf::Vector2f(50,530));
    angle_graph.setTexture(&graph_texture);


    sf::ConvexShape angle_arrow;
    angle_arrow.setPointCount(4);
    angle_arrow.setPoint(0,sf::Vector2f(50,455));
    angle_arrow.setPoint(1,sf::Vector2f(170,455));
    angle_arrow.setPoint(2,sf::Vector2f(170,485));
    angle_arrow.setPoint(3,sf::Vector2f(50,485));


    sf::Vector2f newOrigin = sf::Vector2f(110,470);
    auto offset = newOrigin - angle_arrow.getOrigin();
    angle_arrow.setOrigin(newOrigin);
    angle_arrow.move(offset);

    angle_arrow.setTexture(&angle_arrow_texture);

    sf::ConvexShape angle_whiteout;
    angle_whiteout.setPointCount(4);
    angle_whiteout.setPoint(0,sf::Vector2f(40,365));
    angle_whiteout.setPoint(1,sf::Vector2f(380,365));
    angle_whiteout.setPoint(2,sf::Vector2f(380,530));
    angle_whiteout.setPoint(3,sf::Vector2f(40,530));
    angle_whiteout.setFillColor(sf::Color(255,255,255,100));

    sf::Text angle_ifnot4_text = matlab_ifno_text;
    sf::Text angle_ifnot4_text_2 = angle_ifnot4_text;

    angle_ifnot4_text.setPosition(180,430);
    angle_ifnot4_text_2.setPosition(180,442);
    angle_ifnot4_text.setString("Arbitrary angle selection only  ");
    angle_ifnot4_text_2.setString("possible with fourth function.");

    Vector Beta(2);




    ////////////////////////////
    //Resolution Select Slider//
    ////////////////////////////

    int systemsize = 8;
    bool  isGrabbed_resolution_button = false;

    sf::Text resolution_slider_text = angle_slider_text;
    resolution_slider_text.setString("Mesh Size");
    resolution_slider_text.setPosition(40,540);


    sf::ConvexShape resolution_slider_bar;
    resolution_slider_bar.setPointCount(4);
    resolution_slider_bar.setPoint(0,sf::Vector2f(40,565));
    resolution_slider_bar.setPoint(1,sf::Vector2f(360,565));
    resolution_slider_bar.setPoint(2,sf::Vector2f(360,610));
    resolution_slider_bar.setPoint(3,sf::Vector2f(40,610));
    resolution_slider_bar.setTexture(&sliderbar_resolution);

    sf::CircleShape resolution_slider_button(8);
    resolution_slider_button.setTexture(&slider_button);
    resolution_slider_button.setPosition(sf::Vector2f(114.5,579));

    sf::Text resolution_slider_resolution_text;
    resolution_slider_resolution_text.setFont(calibri);
    resolution_slider_resolution_text.setFillColor(sf::Color::Black);
    resolution_slider_resolution_text.setCharacterSize(16);
    resolution_slider_resolution_text.setPosition(40,615);
    resolution_slider_resolution_text.setString("System Size: 2");

    sf::Text resolution_warning_text = matlab_ifno_text;
    sf::Text resolution_warning_text_2 = resolution_warning_text;

    resolution_warning_text.setPosition(160,615);
    resolution_warning_text_2.setPosition(160,628);
    resolution_warning_text.setString("WARNING: System Sizes of 16+ can");
    resolution_warning_text_2.setString("drastically increase computation time.");


    //////////////////
    //System Preview//
    //////////////////

    sf::Text trial_space_text = title_text;
    trial_space_text.setCharacterSize(22);
    //trial_space_text.setFont(calibri);
    trial_space_text.setString("Trial Space:");
    trial_space_text.setPosition(386,40);

    sf::Text test_space_text = trial_space_text;
    test_space_text.setString("Test Space:");
    test_space_text.setPosition(386,410);

    sf::ConvexShape trial_space_preview;
    trial_space_preview.setPointCount(4);
    trial_space_preview.setPoint(0,sf::Vector2f(340,40));
    trial_space_preview.setPoint(1,sf::Vector2f(740,40));
    trial_space_preview.setPoint(2,sf::Vector2f(740,440));
    trial_space_preview.setPoint(3,sf::Vector2f(340,440));
    trial_space_preview.setTexture(&unrefined_2);

    sf::ConvexShape test_space_preview;
    test_space_preview.setPointCount(4);
    test_space_preview.setPoint(0,sf::Vector2f(340,410));
    test_space_preview.setPoint(1,sf::Vector2f(740,410));
    test_space_preview.setPoint(2,sf::Vector2f(740,810));
    test_space_preview.setPoint(3,sf::Vector2f(340,810));
    test_space_preview.setTexture(&redrefined_2);

    ////////////////////////
    //Generate Data Button//
    ////////////////////////

    sf::ConvexShape generate_data_button;
    generate_data_button.setPointCount(4);
    generate_data_button.setPoint(0,sf::Vector2f(200,728));
    generate_data_button.setPoint(1,sf::Vector2f(360,728));
    generate_data_button.setPoint(2,sf::Vector2f(360,768));
    generate_data_button.setPoint(3,sf::Vector2f(200,768));
    generate_data_button.setTexture(&rectangular_button_unselected);

    sf::Text generate_data_text;
    generate_data_text.setFont(calibri);
    generate_data_text.setCharacterSize(18);
    generate_data_text.setFillColor(sf::Color::Black);
    generate_data_text.setPosition(sf::Vector2f(225,737));
    generate_data_text.setString("Generate Data");



    //////////////////
    //L2 Norm Errors//
    //////////////////

    string Errors1 = "L2 Norm of Absolute Error: ", Errors2 = "L2 Norm of dr/dy: ";
    sf::Text L2_Absolute = generate_data_text;
    L2_Absolute.setCharacterSize(16);
    L2_Absolute.setPosition(sf::Vector2f(40,660));
    L2_Absolute.setString(Errors1);

    sf::Text L2_drdy = L2_Absolute;
    L2_drdy.setPosition(sf::Vector2f(40,680));
    L2_drdy.setString(Errors2);




    //////////////////////////////////
    //                              //
    //  PROCESS LOOP STARTS HERE    //
    //                              //
    //////////////////////////////////

    int framebuffer = 0;

    while (window.isOpen())
    {
        //Process Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if (event.type == sf::Event::Resized){
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }


        ///////////////////
        //Matlab Switches//
        ///////////////////
        if(!isGrabbed_angle_button && !isGrabbed_resolution_button){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Circle(&matlab_no_select,&window)){
                if(matlab_installed){
                    matlab_installed = false;
                    matlab_no_select.setTexture(&button_selected);
                    matlab_yes_select.setTexture(&button_unselected);

                    framebuffer = 10;
                }
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Circle(&matlab_yes_select,&window)){
                if(!matlab_installed){
                    matlab_installed = true;
                    matlab_no_select.setTexture(&button_unselected);
                    matlab_yes_select.setTexture(&button_selected);

                    framebuffer = 10;
                }
            }
        }



        ////////////////////
        //Function Buttons//
        ////////////////////

        if(!isGrabbed_angle_button && !isGrabbed_resolution_button){

            //Click Behaviour
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&function1_button,&window) && selection != 1){

                //Hard reset on angle
                if(selection == 4){
                    angle_slider_button.setPosition(273, angle_slider_button.getPosition().y);
                }

                selection = 1;
                function1_button.setTexture(&rectangular_button_selected);
                function2_button.setTexture(&rectangular_button_unselected);
                function3_button.setTexture(&rectangular_button_unselected);
                function4_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&function2_button,&window) && selection != 2){

                if(selection == 4){
                    angle_slider_button.setPosition(273, angle_slider_button.getPosition().y);
                }

                selection = 2;
                function2_button.setTexture(&rectangular_button_selected);
                function1_button.setTexture(&rectangular_button_unselected);
                function3_button.setTexture(&rectangular_button_unselected);
                function4_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&function3_button,&window) && selection != 3){

                if(selection == 4){
                    angle_slider_button.setPosition(273, angle_slider_button.getPosition().y);
                }

                selection = 3;
                function3_button.setTexture(&rectangular_button_selected);
                function1_button.setTexture(&rectangular_button_unselected);
                function2_button.setTexture(&rectangular_button_unselected);
                function4_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&function4_button,&window) && selection != 4){
                selection = 4;
                function4_button.setTexture(&rectangular_button_selected);
                function1_button.setTexture(&rectangular_button_unselected);
                function2_button.setTexture(&rectangular_button_unselected);
                function3_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }

            //Hover Behaviour
            if(isMouseOver_Rectangle(&function1_button,&window) && selection !=1){
                function1_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&function1_button,&window) && selection !=1){
                function1_button.setTexture(&rectangular_button_unselected);
            }
            if(isMouseOver_Rectangle(&function2_button,&window) && selection != 2){
                function2_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&function2_button,&window) && selection !=2){
                function2_button.setTexture(&rectangular_button_unselected);
            }
            if(isMouseOver_Rectangle(&function3_button,&window) && selection != 3){
                function3_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&function3_button,&window) && selection !=3){
                function3_button.setTexture(&rectangular_button_unselected);
            }
            if(isMouseOver_Rectangle(&function4_button,&window) && selection != 4){
                function4_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&function4_button,&window) && selection !=4){
                function4_button.setTexture(&rectangular_button_unselected);
            }

        }



        ///////////////////////////////
        //Refinement Option Behaviour//
        ///////////////////////////////

        if(!isGrabbed_angle_button && !isGrabbed_resolution_button){

            //Click Behaviour
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&redrefine_button,&window) && refinement_scheme != 1){
                refinement_scheme = 1;
                redrefine_button.setTexture(&rectangular_button_selected);
                linerefine_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&linerefine_button,&window) && refinement_scheme != 2 && selection != 4){
                refinement_scheme = 2;
                linerefine_button.setTexture(&rectangular_button_selected);
                redrefine_button.setTexture(&rectangular_button_unselected);

                framebuffer = 10;
            }

            //Hover Behaviour
            if(isMouseOver_Rectangle(&redrefine_button,&window) && refinement_scheme!=1){
                redrefine_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&redrefine_button,&window) && refinement_scheme !=1){
                redrefine_button.setTexture(&rectangular_button_unselected);
            }
            if(isMouseOver_Rectangle(&linerefine_button,&window) && refinement_scheme!=2 && selection != 4){
                linerefine_button.setTexture(&rectangular_button_hover);
            }
            else if(!isMouseOver_Rectangle(&linerefine_button,&window) && refinement_scheme !=2){
                linerefine_button.setTexture(&rectangular_button_unselected);
            }

            //This deals with if function select is switched to the fourth option (where vertical refinement is not possible) while it is selected
            if(selection == 4 && refinement_scheme == 2){
                refinement_scheme = 1;
                redrefine_button.setTexture(&rectangular_button_selected);
                linerefine_button.setTexture(&rectangular_button_unselected);
            }

        }



        //////////////////////////
        //Angle Selection Slider//
        //////////////////////////

        //The slider has two behaviours - one "grabbed" behaviour, where if the user holds the mouse input through a movement, the slider button will follow the mouse's movement regardless of its y position
        // (as long as the mouse is still being held down)
        //The other i'll call 'click to' where the button snaps to an area on the slider clicked by the user if the button is not currently there. This also automatically grabs the button on the next frame, which means you
        //don't have to write any extra code to tell it to do that. Pretty neat

        //"Grabbed" behaviour
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isGrabbed_angle_button && selection == 4 && !isGrabbed_resolution_button){
            if(mousePosition.x - 8 < 40){
                angle_slider_button.setPosition(40, angle_slider_button.getPosition().y);
            }
            else if(mousePosition.x  - 8 > 350){
                angle_slider_button.setPosition(350, angle_slider_button.getPosition().y);
            }
            else{
                angle_slider_button.setPosition(mousePosition.x - 8, angle_slider_button.getPosition().y);
            }

        }
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && selection == 4 && !isGrabbed_resolution_button){
            if(isMouseOver_Circle(&angle_slider_button,&window)){
                isGrabbed_angle_button = true;
            }
        }
        else{
            isGrabbed_angle_button = false;
        }

        //"Click to" behaviour
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseOver_Circle(&angle_slider_button,&window) && selection == 4 && !isGrabbed_resolution_button
           && mousePosition.x - 8 >= 40 && mousePosition.x - 8 <= 350
           && mousePosition.y >= 375 && mousePosition.y <= 400){
                angle_slider_button.setPosition(mousePosition.x - 8, angle_slider_button.getPosition().y);
        }


        //Angle Text/Arrow update
        angle = floor(360.0*angle_slider_button.getPosition().x/310.0 - 1440.0/31.0);
        string angle_text = to_string(angle);
        angle_text.erase(angle_text.find_last_not_of('0') + 1, std::string::npos);
        angle_text.erase(angle_text.find_last_not_of('.') + 1, std::string::npos);
        angle_text += "°";
        angle_slider_angle_text.setString(angle_text);

        angle_arrow.setRotation(angle);

        Beta[0] = round(cos(angle*M_PI/180.0) * 100) / 100;
        Beta[1] = round(-sin(angle*M_PI/180.0) * 100) / 100;


        /////////////////////
        //Resolution Slider//
        /////////////////////

        //"Grabbed" behaviour
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isGrabbed_resolution_button && !isGrabbed_angle_button){
            if(mousePosition.x - 8 < 37.5){
                resolution_slider_button.setPosition(37.5, resolution_slider_button.getPosition().y);
            }
            else if(mousePosition.x  - 8 > 346.5){
                resolution_slider_button.setPosition(346.5, resolution_slider_button.getPosition().y);
            }
            else{
                resolution_slider_button.setPosition(mousePosition.x, resolution_slider_button.getPosition().y);
            }

        }
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isGrabbed_angle_button){
            if(isMouseOver_Circle(&resolution_slider_button,&window)){
                isGrabbed_resolution_button = true;
            }
        }
        else{
            isGrabbed_resolution_button = false;
        }

        //"Click to" behaviour
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseOver_Circle(&resolution_slider_button,&window) && !isGrabbed_angle_button
           && mousePosition.x - 8 >= 37.5 && mousePosition.x - 8 <= 346.5
           && mousePosition.y >= 575 && mousePosition.y <= 600){
                resolution_slider_button.setPosition(mousePosition.x - 8, resolution_slider_button.getPosition().y);
                isGrabbed_resolution_button = true;
        }

        //"Snap to" behaviour
        if(!isGrabbed_resolution_button ){
            sf::Vector2f button_position = resolution_slider_button.getPosition();
            if(button_position.x < 76){
                resolution_slider_button.setPosition(37.5, resolution_slider_button.getPosition().y);
                systemsize = 1;
                resolution_slider_resolution_text.setString("System Size: 1");
            }
            else if(button_position.x < 153.5){
                resolution_slider_button.setPosition(114.5, resolution_slider_button.getPosition().y);
                systemsize = 2;
                resolution_slider_resolution_text.setString("System Size: 2");
            }
            else if(button_position.x < 231){
                resolution_slider_button.setPosition(192.5, resolution_slider_button.getPosition().y);
                systemsize = 4;
                resolution_slider_resolution_text.setString("System Size: 4");
            }
            else if(button_position.x < 308){
                resolution_slider_button.setPosition(269.5, resolution_slider_button.getPosition().y);
                systemsize = 8;
                resolution_slider_resolution_text.setString("System Size: 8");
            }
            else{
                resolution_slider_button.setPosition(346.5, resolution_slider_button.getPosition().y);
                systemsize = 16;
                resolution_slider_resolution_text.setString("System Size: 16");
            }
        }

        ////////////////////////////
        //System Preview Behaviour//
        ////////////////////////////

        if(systemsize == 1){
            trial_space_preview.setTexture(&unrefined_1);
            if(refinement_scheme == 1){
                test_space_preview.setTexture(&redrefined_1);
            }
            else{
                test_space_preview.setTexture(&verticalrefined_1);
            }
        }
        else if(systemsize == 2){
            trial_space_preview.setTexture(&unrefined_2);
            if(refinement_scheme == 1){
                test_space_preview.setTexture(&redrefined_2);
            }
            else{
                test_space_preview.setTexture(&verticalrefined_2);
            }
        }
        else if(systemsize == 4){
            trial_space_preview.setTexture(&unrefined_4);
            if(refinement_scheme == 1){
                test_space_preview.setTexture(&redrefined_4);
            }
            else{
                test_space_preview.setTexture(&verticalrefined_4);
            }
        }
        else if(systemsize == 8){
            trial_space_preview.setTexture(&unrefined_8);
            if(refinement_scheme == 1){
                test_space_preview.setTexture(&redrefined_8);
            }
            else{
                test_space_preview.setTexture(&verticalrefined_8);
            }
        }
        else if(systemsize == 16){
                        trial_space_preview.setTexture(&unrefined_16);
            if(refinement_scheme == 1){
                test_space_preview.setTexture(&redrefined_16);
            }
            else{
                test_space_preview.setTexture(&verticalrefined_16);
            }
        }


        ////////////////////////
        //Generate Data Button//
        ////////////////////////
        if(!isGrabbed_angle_button && !isGrabbed_resolution_button){

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && framebuffer == 0 && isMouseOver_Rectangle(&generate_data_button,&window)){
                generate_data_button.setTexture(&rectangular_button_selected);
                window.draw(generate_data_button);
                window.draw(generate_data_text);
                window.display();

                Vector Errors(2);
                Errors = DDMRes_Peterson(systemsize,selection,refinement_scheme,Beta);
                Errors1 = "L2 Norm of Absolute Error: " + to_string(Errors[0]);
                Errors2 = "L2 Norm of dr/dy: " + to_string(Errors[1]);

                L2_Absolute.setString(Errors1);
                L2_drdy.setString(Errors2);

                if(matlab_installed){
                    if(!matlab_running){
                        matlab_running = true;
                        std::thread t([&matlab_running]{
                                    system("matlab -batch \"Plotting\" &");
                                    matlab_running = false;
                                    });

                        t.detach();
                    }

                }

                framebuffer = 10;
            }

            //Hover Behaviour//
            if(isMouseOver_Rectangle(&generate_data_button,&window)){
                generate_data_button.setTexture(&rectangular_button_hover);
            }
            else{
                generate_data_button.setTexture(&rectangular_button_unselected);
            }

        }



        if(framebuffer > 0){framebuffer--;}

        /////////////////////
        //Drawing the frame//
        /////////////////////

        window.clear();

        //Background & Title
        window.draw(background);
        window.draw(title_text);

        //System Preview
        window.draw(trial_space_preview);
        window.draw(test_space_preview);
        window.draw(trial_space_text);
        window.draw(test_space_text);

        //Matlab Select
        window.draw(matlab_text);
        window.draw(matlab_text_2);
        window.draw(matlab_yes_select);
        window.draw(matlab_no_select);
        if(!matlab_installed){
            window.draw(matlab_ifno_text);
        }

        //Function Select
        window.draw(function_select_text);
        window.draw(function1_button);
        window.draw(function1_text);
        window.draw(function2_button);
        window.draw(function2_text);
        window.draw(function3_button);
        window.draw(function3_text);
        window.draw(function4_button);
        window.draw(function4_text);

        //Refinement Select
        window.draw(refinement_select_text);
        window.draw(redrefine_button);
        window.draw(redrefine_text);
        window.draw(linerefine_button);
        window.draw(linerefine_text);
        if(selection == 4){
            window.draw(WhiteOut_VerticalLine);
        }


        //Angle Slider
        window.draw(angle_slider_text);
        window.draw(angle_slider_bar);
        window.draw(angle_slider_button);
        window.draw(angle_slider_angle_text);
        window.draw(angle_graph);
        window.draw(angle_arrow);
        if(selection != 4){
            window.draw(angle_whiteout);
            window.draw(angle_ifnot4_text);
            window.draw(angle_ifnot4_text_2);
        }

        //Resolution slider
        window.draw(resolution_slider_text);
        window.draw(resolution_slider_bar);
        window.draw(resolution_slider_button);
        window.draw(resolution_slider_resolution_text);
        if(systemsize == 16){
            window.draw(resolution_warning_text);
            window.draw(resolution_warning_text_2);
        }


        //Generate Data
        window.draw(generate_data_button);
        window.draw(generate_data_text);

        //Errors Text
        window.draw(L2_Absolute);
        window.draw(L2_drdy);

        //Border - this goes last so as to be on top of any extraneous white backgrounds
        window.draw(border);

        window.display();
    }

    return 0;
}


//This method runs the DDMRes routine that was used for my dissertation. The process is O(n^5) due to the inefficient gaussian quadrature algorithm, so large n values take a very long time to evaluate
Vector DDMRes_Peterson(int n,int selection,int refinement_scheme, Vector Beta){

    //Setting up test space mesh
    Mesh test_space;

    test_space.Peterson(n);

    if(refinement_scheme == 1){
        test_space.Refine();
    }
    else{
        test_space.Vertical_Refine(n);
    }


    //Setting up the basis functions associated with the test space mesh
    BasisFunc test_space_basis[test_space.getNum_Vertices()];
    int numnodes, numelements, writepos, writepos_connectivity;
    int alreadyusedwrite;
    bool used;

    //This loop assigns the basis functions with their nodes in a specific (to them) array, and elements, in the form of a local connectivity array
    Vector AlreadyUsed(10);
    vector<int> indices;
    for(int i = 0; i < test_space.getNum_Vertices(); i++){
        numelements = 0;
        numnodes = 1;
        writepos = 0;
        writepos_connectivity = 0;
        Matrix Connectivity_x, Connectivity_y;
        Matrix Nodes;


        alreadyusedwrite = 0;
        for(int j = 0; j < 10; j++){
            AlreadyUsed[j] = 0;
        }

        //Finding the number of elements/nodes associated with basis function
        for(int j = 0; j < test_space.getNum_Elements(); j++){
            if(test_space.getConnectivity(0,j) == i+1 || test_space.getConnectivity(1,j) == i+1 || test_space.getConnectivity(2,j) == i+1){
                numelements++;

                if(test_space.getConnectivity(0,j) == i+1){
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(1,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(1,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(2,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(2,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }

                }

                if(test_space.getConnectivity(1,j) == i+1){
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(0,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(0,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(2,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(2,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }

                }

                if(test_space.getConnectivity(2,j) == i+1){
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(1,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(1,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }
                    used = false;
                    for(int k = 0; k < AlreadyUsed.GetSize(); k++){
                        if(AlreadyUsed.Read(k) == test_space.getConnectivity(0,j)){
                            used = true;
                        }
                    }
                    if(!used){
                        AlreadyUsed[alreadyusedwrite] = test_space.getConnectivity(0,j);
                        alreadyusedwrite++;
                        numnodes++;
                    }

                }
            }
        }

        Nodes.SetRectangle(2,numnodes);

        Connectivity_x.SetRectangle(3,numelements);
        Connectivity_y.SetRectangle(3,numelements);

        //writing the center node
        Nodes.Write(0,writepos,test_space.getVertex(0,i));
        Nodes.Write(1,writepos,test_space.getVertex(1,i));
        writepos++;

        //finding the rest of the nodes
        for(int j = 0; j < test_space.getNum_Elements(); j++){

            if(test_space.getConnectivity(0,j) == i+1 || test_space.getConnectivity(1,j) == i+1 || test_space.getConnectivity(2,j) == i+1){

                indices.push_back(test_space.getConnectivity(0,j)-1);
                indices.push_back(test_space.getConnectivity(1,j)-1);
                indices.push_back(test_space.getConnectivity(2,j)-1);

                for(int k = 0; k < 3; k++){
                    Connectivity_x.Write(k,writepos_connectivity,test_space.getVertex(0,test_space.getConnectivity(k,j)-1));
                    Connectivity_y.Write(k,writepos_connectivity,test_space.getVertex(1,test_space.getConnectivity(k,j)-1));

                }
                writepos_connectivity++;

            }
        }



        //Removing duplicate node indices - this also sorts them in order of their size but it doesn't matter, we need to order them differently anyway
        sort(indices.begin(), indices.end());
        indices.erase(unique(indices.begin(),indices.end()),indices.end());

        //now removing the center index from the list (it's already been written in)
        indices.erase(std::remove(indices.begin(), indices.end(), i), indices.end());

        //loop over all the nodes and add them clockwise in terms of their angle relative to the center node
        for(int j = 0; j < (int)indices.size(); j++){
                if(atan2(test_space.getVertex(1,indices[j]) - test_space.getVertex(1,i),test_space.getVertex(0,indices[j]) - test_space.getVertex(0,i)) == 0){
                Nodes.Write(0,writepos,test_space.getVertex(0,indices[j]));
                Nodes.Write(1,writepos,test_space.getVertex(1,indices[j]));
                indices.erase(indices.begin() + j);
                writepos++;
                }
        }
        bool negatives = true;
        while(negatives){
            int lowestnegative = 5; int lowestindex = -1; double angle;
            for(int j = 0; j < (int)indices.size(); j++){
                angle = atan2(test_space.getVertex(1,indices[j]) - test_space.getVertex(1,i),test_space.getVertex(0,indices[j]) - test_space.getVertex(0,i));
                if(angle < 0){
                    if(abs(angle) < abs(lowestnegative)){
                        lowestnegative = angle;
                        lowestindex = j;
                    }
                }
            }
            if(lowestindex != -1){
                Nodes.Write(0,writepos,test_space.getVertex(0,indices[lowestindex]));
                Nodes.Write(1,writepos,test_space.getVertex(1,indices[lowestindex]));
                indices.erase(indices.begin() + lowestindex);
                writepos++;
            }
            else{
                negatives = false;
            }

        }



        bool positives = true;
        while(positives){
            int highestpositive = 0; int highestindex = -1; double angle;
            for(int j = 0; j < (int)indices.size(); j++){
                angle = atan2(test_space.getVertex(1,indices[j]) - test_space.getVertex(1,i),test_space.getVertex(0,indices[j]) - test_space.getVertex(0,i));
                if(angle > 0){
                    if(abs(angle) > abs(highestpositive)){
                        highestpositive = angle;
                        highestindex = j;
                    }
                }
            }
            if(highestindex != -1){
                Nodes.Write(0,writepos,test_space.getVertex(0,indices[highestindex]));
                Nodes.Write(1,writepos,test_space.getVertex(1,indices[highestindex]));
                indices.erase(indices.begin() + highestindex);
                writepos++;
            }
            else{
                positives = false;
            }
        }



        test_space_basis[i].SetNodes(numnodes,Nodes);

        //Setting up Local connectivity matrix for each basis function

        test_space_basis[i].SetupConnectivity(numelements,Connectivity_x,Connectivity_y);


    }

    //Setting up the U mesh
    Mesh trial_space;
    trial_space.Peterson(n);

    //Setting up the Square LHS Matrix
    Matrix Square(test_space.getNum_Vertices());
    for(int i = 0; i < test_space.getNum_Vertices(); i++){
        for(int j = 0; j < test_space.getNum_Vertices();j++){
            Square.Write(i,j,test_space_basis[i].squareIntegral(Beta,test_space_basis[j]));
        }
    }

    //Now we loop over the Square and remove any elements corresponding to outflow nodes
    Vector center(2);
    vector<int> indices_to_remove;

    int num_indexes_removed = 0;
    for(int j = 0; j < test_space.getNum_Vertices(); j++){
        center = test_space_basis[j].getNode(0);
        if(isOnBoundary(center)){
            if(Beta.ScalarProduct(getUnitNormal(center)) > 0 || isSpecialCase(Beta,center)){
                //outflow, so remove part
                Square.RemoveIndex(j - num_indexes_removed);
                num_indexes_removed++;
                indices_to_remove.push_back(j);
            }
        }
    }


    //Setting up the Rectangular LHS Matrices
    Matrix Rectangle(trial_space.getNum_Elements(),test_space.getNum_Vertices());
    for(int i = 0; i < trial_space.getNum_Elements(); i++){
        Matrix Triangle(2,3);
        for(int j = 0; j < test_space.getNum_Vertices(); j++){

            for(int k = 0; k < 3; k++){
                Triangle.Write(0,k,trial_space.getVertex(0,trial_space.getConnectivity(k,i)-1));
                Triangle.Write(1,k,trial_space.getVertex(1,trial_space.getConnectivity(k,i)-1));
            }

            Rectangle.Write(i,j,test_space_basis[j].rectangleIntegral(Beta,Triangle));
        }


    }


    //Removing the right elements
    num_indexes_removed = 0;
    for(int i = 0; i < (int)indices_to_remove.size(); i++){
        Rectangle.RemoveRow(indices_to_remove[i] - num_indexes_removed);
        num_indexes_removed++;
    }

    //Combining for full LHS
    Matrix Full_LHS(Square.mSize_x + Rectangle.mSize_x);
    Full_LHS.Insert(0,0,Square);
    Full_LHS.Insert(Square.mSize_x,0,Rectangle);
    Rectangle.Transpose();
    Full_LHS.Insert(0,Square.mSize_x,Rectangle);

    //Setting up RHS
    Vector RHS(test_space.getNum_Vertices() + trial_space.getNum_Elements());
    Vector *pRHS = &RHS;


    //Only elements up to test space number vertices have nonzero value
    //Adding F term first - This does not care about the flow direction

    for(int i = 0; i < test_space.getNum_Vertices(); i++){
        RHS[i] += source_integral(test_space_basis[i],source_integrand,selection);
    }

    //Adding G Term

    Vector Start(2),End(2);
    Vector Unit(2);
    Unit[0] = 1; Unit[1] = 1;
    Vector unit_normal(2);

    for(int i = 0; i < test_space.getNum_Vertices(); i++){
        //Check if the center node of the test function is on the boundary
        if(isOnBoundary(test_space_basis[i].getNode(0))){
            //Check if it is an inflow node
            //if(i == 2){cout << "i =3, " << Flow_Normal_Product(test_space_basis[i],Beta) << endl;}
            if(Flow_Normal_Product(test_space_basis[i],Beta) < 0 || isSpecialCase(Beta,test_space_basis[i].getNode(0))){
                for(int j = 1; j < test_space_basis[i].getNumNodes();j++){
                    if(isOnBoundary(test_space_basis[i].getNode(j))){
                        if(Beta.ScalarProduct(getUnitNormal(test_space_basis[i].getNode(j))) < 0 || isSpecialCase(Beta,test_space_basis[i].getNode(j))){
                            //this condition checks which direction to integrate over

                            if(test_space_basis[i].getNode(0).ScalarProduct(Unit) < test_space_basis[i].getNode(j).ScalarProduct(Unit)){
                                Start = test_space_basis[i].getNode(0);
                                End = test_space_basis[i].getNode(j);
                                if(Start.Read(0) == End.Read(0)){unit_normal[0] = 2*Start.Read(0) - 1; unit_normal[1] = 0;}
                                if(Start.Read(1) == End.Read(1)){unit_normal[1] = 2*Start.Read(1) - 1; unit_normal[0] = 0;}

                                RHS[i] += trapezium_on_boundary(Start,End,boundary_function,test_space_basis[i],Beta,unit_normal,selection);
                            }
                            else{
                                Start = test_space_basis[i].getNode(j);
                                End = test_space_basis[i].getNode(0);
                                if(Start.Read(0) == End.Read(0)){unit_normal[0] = 2*Start.Read(0) - 1; unit_normal[1] = 0;}
                                if(Start.Read(1) == End.Read(1)){unit_normal[1] = 2*Start.Read(1) - 1; unit_normal[0] = 0;}
                                RHS[i] += trapezium_on_boundary(Start,End,boundary_function,test_space_basis[i],Beta,unit_normal,selection);
                            }
                        }
                    }
                }
            }
        }
    }

    //Remove those elements that correspond to outflow nodes
    num_indexes_removed = 0;
    for(int i = 0; i < (int)indices_to_remove.size(); i++){
        RHS.Remove(indices_to_remove[i] - num_indexes_removed);
        num_indexes_removed++;
    }



    //Solving the System
    Vector Solution(Rectangle.mSize_x + Rectangle.mSize_y);
    Vector *pSolution = &Solution;
    Full_LHS.Gauss_Eliminate(pRHS);
    Full_LHS.Back_Substitute(RHS,pSolution);


    Vector U(trial_space.getNum_Elements());

    for(int i = Rectangle.mSize_x; i < Rectangle.mSize_x + Rectangle.mSize_y; i++){
        U[i - (Rectangle.mSize_x)] = Solution.Read(i);
    }

    //Outputting the Results
    //The best x,y co-ordinate for each point would be the centroid of the element

    ofstream outfile;
    outfile.open("Solution.txt");

    Vector coords(2);
    for(int i = 0; i < U.GetSize(); i++){
        coords = trial_space.getMidpoint(i);
        outfile << U.Read(i) << " " << coords[0] << " " << coords[1] << endl;
    }
    outfile.close();


    //Generate L2 Norm of Error

    Vector L2_Norms(2);
    L2_Norms[0] = L2Norm_Error(trial_space,U,selection,Beta);
    L2_Norms[1] = L2Norm_drdy(Solution,Square,U.GetSize());


    return L2_Norms;
}






