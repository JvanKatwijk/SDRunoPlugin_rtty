#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/dragger.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <iunoplugincontroller.h>

// Shouldn't need to change these
#define topBarHeight (27)
#define bottomBarHeight (8)
#define sideBorderWidth (8)

// TODO: Change these numbers to the height and width of your form
//#define formWidth (297)
#define formWidth (550)
#define formHeight (240)

class SDRunoPlugin_rttyUi;

class SDRunoPlugin_rttyForm : public nana::form {
public:

	SDRunoPlugin_rttyForm (SDRunoPlugin_rttyUi& parent,
	                          IUnoPluginController& controller);		
	~SDRunoPlugin_rttyForm ();
	
void	set_rttyAfcon		(const std::string &);
void	set_rttyReverse		(const std::string &);
void	set_rttyShift		(const std::string &);
void	set_rttyBaudrate	(const std::string &);
void	set_rttyParity		(const std::string &);
void	set_rttyMsb		(const std::string &);
void	set_rttyNbits		(const std::string &);
void	set_rttyStopbits	(const std::string &);

void	set_rttyIF	        (int v);
void	set_freqCorrection	(int v);
void	set_rttyBaudRate	(int v);
void	set_rttyGuess		(int v);
void	set_rttyText		(const std::string &);
void	Run			();

private:

	void Setup();

	// The following is to set up the panel graphic to look like a standard SDRuno panel
	nana::picture bg_border{ *this, nana::rectangle(0, 0, formWidth, formHeight) };
	nana::picture bg_inner{ bg_border, nana::rectangle(sideBorderWidth, topBarHeight, formWidth - (2 * sideBorderWidth), formHeight - topBarHeight - bottomBarHeight) };
	nana::picture header_bar{ *this, true };
	nana::label title_bar_label{ *this, true };
	nana::dragger form_dragger;
	nana::label form_drag_label{ *this, nana::rectangle(0, 0, formWidth, formHeight) };
	nana::paint::image img_min_normal;
	nana::paint::image img_min_down;
	nana::paint::image img_close_normal;
	nana::paint::image img_close_down;
	nana::paint::image img_header;
	nana::picture close_button {*this, nana::rectangle(0, 0, 20, 15) };
	nana::picture min_button {*this, nana::rectangle(0, 0, 20, 15) };

	// Uncomment the following 4 lines if you want a SETT button and panel
	nana::paint::image img_sett_normal;
	nana::paint::image img_sett_down;
	nana::picture sett_button{ *this, nana::rectangle(0, 0, 40, 15) };
	void SettingsButton_Click();

	// TODO: Now add your UI controls here
//
//	first the number displays
	nana::label  strengthMeter {*this, nana::rectangle (30, 50, 30, 20) };
	nana::label  freqCorrection {*this, nana::rectangle (30, 80, 30, 20) };
	nana::label  rttyBaudRate  {*this, nana::rectangle (30, 120, 30, 20) };
	nana::label  rttyGuess     {*this, nana::rectangle (30, 160, 30, 20) };

//
//	then the selectors
	nana::label  xx1           { *this, nana::rectangle(60, 30, 90, 15) };
	nana::combox widthSelector {*this, nana::rectangle (60, 50, 90, 20) };

	nana::label  xx2           { *this, nana::rectangle(170, 30, 90, 15) };
	nana::combox rateSelector  {*this, nana::rectangle (170, 50, 90, 20) };

	nana::label  xx3           { *this, nana::rectangle(280, 30, 90,  15) };
	nana::combox bitSelector   {*this, nana::rectangle (280, 50, 90, 20)};

	nana::label  xx4           {*this,nana::rectangle  (390, 30, 90, 15) }; 
	nana::combox stopbitsSelector {*this, nana::rectangle (390, 50, 90, 20) };

	nana::label xx5            { *this, nana::rectangle(60, 70, 90, 15) };
	nana::combox paritySelector {*this, nana::rectangle (60, 90, 90, 20) };

	nana::label xx6           {*this, nana::rectangle (170, 70, 90, 15) };
	nana::combox msbSelector  {*this, nana::rectangle (170, 90, 90, 20) };

	nana::label xx7           {*this, nana::rectangle (280,  70, 90, 15) };
	nana::combox afcSelector  {*this, nana::rectangle (280,  90, 90, 20) };

	nana::label xx8           {*this, nana::rectangle (390, 70, 90, 15) };
	nana::combox normalSelector {*this, nana::rectangle (390, 90, 90, 20)};

	nana::label rttyText {*this, nana::rectangle (20, 200, 450, 20) };
	SDRunoPlugin_rttyUi & m_parent;
	IUnoPluginController & m_controller;
};
