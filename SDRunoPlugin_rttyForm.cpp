#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#endif

#include "SDRunoPlugin_rttyForm.h"
#include "SDRunoPlugin_rttyUi.h"
#include "resource.h"
#include <io.h>
#include <shlobj.h>

void	SDRunoPlugin_rttyForm::set_rttyAfcon	(const std::string &s) {
	m_parent. set_rttyAfcon (s);
}
void	SDRunoPlugin_rttyForm::set_rttyReverse	(const std::string &s) {
	m_parent. set_rttyReverse (s);
}

void	SDRunoPlugin_rttyForm::set_rttyShift	(const std::string &s) {
	m_parent. set_rttyShift (s);
}

void	SDRunoPlugin_rttyForm::set_rttyBaudrate	(const std::string &s) {
	m_parent. set_rttyBaudrate (s);
}

void	SDRunoPlugin_rttyForm::set_rttyParity	(const std::string &s) {
	m_parent. set_rttyParity (s);
}

void	SDRunoPlugin_rttyForm::set_rttyMsb		(const std::string &s) {
	m_parent. set_rttyMsb (s);
}

void	SDRunoPlugin_rttyForm::set_rttyNbits	(const std::string &s) {
	m_parent. set_rttyNbits (s);
}

void	SDRunoPlugin_rttyForm::set_rttyStopbits	(const std::string &s) {
	m_parent. set_rttyStopbits (s);
}

void	SDRunoPlugin_rttyForm::show_rttyIF	(int v) {
	strengthMeter. caption (std::to_string (v));
}

void	SDRunoPlugin_rttyForm::show_freqCorrection	(int v) {
	freqCorrection. caption (std::to_string (v));
}

void	SDRunoPlugin_rttyForm::show_rttyBaudRate	(int v) {
	rttyBaudRate. caption (std::to_string (v));
}

void	SDRunoPlugin_rttyForm::show_rttyGuess	(int v) {
	rttyGuess. caption (std::to_string (v));
}

void	SDRunoPlugin_rttyForm::show_rttyText	(const std::string &s) {
	rttyText. caption (s);
}


// Form constructor with handles to parent and uno controller - launches form Setup
	SDRunoPlugin_rttyForm::SDRunoPlugin_rttyForm(
	SDRunoPlugin_rttyUi& parent,
	IUnoPluginController& controller) :
	nana::form(nana::API::make_center(formWidth, formHeight),
		nana::appearance(false, true, false, false, true, false, false)),
	m_parent(parent),
	m_controller(controller) {
	Setup();
}

// Form deconstructor
	SDRunoPlugin_rttyForm::~SDRunoPlugin_rttyForm () {

}

// Start Form and start Nana UI processing
void	SDRunoPlugin_rttyForm::Run() {
	show();
	nana::exec();
}

// Create the initial plugin form
void	SDRunoPlugin_rttyForm::Setup() {
	// This first section is all related to the background and border
	// it shouldn't need to be changed
	nana::paint::image img_border;
	nana::paint::image img_inner;
	HMODULE hModule = NULL;
	HRSRC rc_border = NULL;
	HRSRC rc_inner = NULL;
	HRSRC rc_close = NULL;
	HRSRC rc_close_over = NULL;
	HRSRC rc_min = NULL;
	HRSRC rc_min_over = NULL;
	HRSRC rc_bar = NULL;
	HRSRC rc_sett = NULL;
	HRSRC rc_sett_over = NULL;
	HBITMAP bm_border = NULL;
	HBITMAP bm_inner = NULL;
	HBITMAP bm_close = NULL;
	HBITMAP bm_close_over = NULL;
	HBITMAP bm_min = NULL;
	HBITMAP bm_min_over = NULL;
	HBITMAP bm_bar = NULL;
	HBITMAP bm_sett = NULL;
	HBITMAP bm_sett_over = NULL;
	BITMAPINFO bmInfo_border = { 0 };
	BITMAPINFO bmInfo_inner = { 0 };
	BITMAPINFO bmInfo_close = { 0 };
	BITMAPINFO bmInfo_close_over = { 0 };
	BITMAPINFO bmInfo_min = { 0 };
	BITMAPINFO bmInfo_min_over = { 0 };
	BITMAPINFO bmInfo_bar = { 0 };
	BITMAPINFO bmInfo_sett = { 0 };
	BITMAPINFO bmInfo_sett_over = { 0 };
	BITMAPFILEHEADER borderHeader = { 0 };
	BITMAPFILEHEADER innerHeader = { 0 };
	BITMAPFILEHEADER closeHeader = { 0 };
	BITMAPFILEHEADER closeoverHeader = { 0 };
	BITMAPFILEHEADER minHeader = { 0 };
	BITMAPFILEHEADER minoverHeader = { 0 };
	BITMAPFILEHEADER barHeader = { 0 };
	BITMAPFILEHEADER settHeader = { 0 };
	BITMAPFILEHEADER settoverHeader = { 0 };
	HDC hdc = NULL;
	BYTE* borderPixels = NULL;
	BYTE* innerPixels = NULL;
	BYTE* closePixels = NULL;
	BYTE* closeoverPixels = NULL;
	BYTE* minPixels = NULL;
	BYTE* minoverPixels = NULL;
	BYTE* barPixels = NULL;
	BYTE* barfocusedPixels = NULL;
	BYTE* settPixels = NULL;
	BYTE* settoverPixels = NULL;
	const unsigned int rawDataOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);
	hModule = GetModuleHandle(L"SDRunoPlugin_rtty");
	hdc = GetDC(NULL);
	rc_border = FindResource(hModule, MAKEINTRESOURCE(IDB_BG_BORDER), RT_BITMAP);
	rc_inner = FindResource(hModule, MAKEINTRESOURCE(IDB_BACKGROUND), RT_BITMAP);
	rc_close = FindResource(hModule, MAKEINTRESOURCE(IDB_CLOSE), RT_BITMAP);
	rc_close_over = FindResource(hModule, MAKEINTRESOURCE(IDB_CLOSE_DOWN), RT_BITMAP);
	rc_min = FindResource(hModule, MAKEINTRESOURCE(IDB_MIN), RT_BITMAP);
	rc_min_over = FindResource(hModule, MAKEINTRESOURCE(IDB_MIN_DOWN), RT_BITMAP);
	rc_bar = FindResource(hModule, MAKEINTRESOURCE(IDB_HEADER), RT_BITMAP);
	rc_sett = FindResource(hModule, MAKEINTRESOURCE(IDB_SETT), RT_BITMAP);
	rc_sett_over = FindResource(hModule, MAKEINTRESOURCE(IDB_SETT_DOWN), RT_BITMAP);
	bm_border = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_BG_BORDER), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_inner = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_BACKGROUND), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_close = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_CLOSE), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_close_over = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_CLOSE_DOWN), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_min = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_MIN), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_min_over = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_MIN_DOWN), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_bar = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_HEADER), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_sett = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_SETT), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bm_sett_over = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(IDB_SETT_DOWN), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	bmInfo_border.bmiHeader.biSize = sizeof(bmInfo_border.bmiHeader);
	bmInfo_inner.bmiHeader.biSize = sizeof(bmInfo_inner.bmiHeader);
	bmInfo_close.bmiHeader.biSize = sizeof(bmInfo_close.bmiHeader);
	bmInfo_close_over.bmiHeader.biSize = sizeof(bmInfo_close_over.bmiHeader);
	bmInfo_min.bmiHeader.biSize = sizeof(bmInfo_min.bmiHeader);
	bmInfo_min_over.bmiHeader.biSize = sizeof(bmInfo_min_over.bmiHeader);
	bmInfo_bar.bmiHeader.biSize = sizeof(bmInfo_bar.bmiHeader);
	bmInfo_sett.bmiHeader.biSize = sizeof(bmInfo_sett.bmiHeader);
	bmInfo_sett_over.bmiHeader.biSize = sizeof(bmInfo_sett_over.bmiHeader);
	GetDIBits(hdc, bm_border, 0, 0, NULL, &bmInfo_border, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_inner, 0, 0, NULL, &bmInfo_inner, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_close, 0, 0, NULL, &bmInfo_close, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_close_over, 0, 0, NULL, &bmInfo_close_over, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_min, 0, 0, NULL, &bmInfo_min, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_min_over, 0, 0, NULL, &bmInfo_min_over, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_bar, 0, 0, NULL, &bmInfo_bar, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_sett, 0, 0, NULL, &bmInfo_sett, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_sett_over, 0, 0, NULL, &bmInfo_sett_over, DIB_RGB_COLORS);
	bmInfo_border.bmiHeader.biCompression = BI_RGB;
	bmInfo_inner.bmiHeader.biCompression = BI_RGB;
	bmInfo_close.bmiHeader.biCompression = BI_RGB;
	bmInfo_close_over.bmiHeader.biCompression = BI_RGB;
	bmInfo_min.bmiHeader.biCompression = BI_RGB;
	bmInfo_min_over.bmiHeader.biCompression = BI_RGB;
	bmInfo_bar.bmiHeader.biCompression = BI_RGB;
	bmInfo_sett.bmiHeader.biCompression = BI_RGB;
	bmInfo_sett_over.bmiHeader.biCompression = BI_RGB;
	borderHeader.bfOffBits = rawDataOffset;
	borderHeader.bfSize = bmInfo_border.bmiHeader.biSizeImage;
	borderHeader.bfType = 0x4D42;
	innerHeader.bfOffBits = rawDataOffset;
	innerHeader.bfSize = bmInfo_inner.bmiHeader.biSizeImage;
	innerHeader.bfType = 0x4D42;
	closeHeader.bfOffBits = rawDataOffset;
	closeHeader.bfSize = bmInfo_close.bmiHeader.biSizeImage;
	closeHeader.bfType = 0x4D42;
	closeoverHeader.bfOffBits = rawDataOffset;
	closeoverHeader.bfSize = bmInfo_close_over.bmiHeader.biSizeImage;
	closeoverHeader.bfType = 0x4D42;
	minHeader.bfOffBits = rawDataOffset;
	minHeader.bfSize = bmInfo_min.bmiHeader.biSizeImage;
	minHeader.bfType = 0x4D42;
	minoverHeader.bfOffBits = rawDataOffset;
	minoverHeader.bfSize = bmInfo_min_over.bmiHeader.biSizeImage;
	minoverHeader.bfType = 0x4D42;
	barHeader.bfOffBits = rawDataOffset;
	barHeader.bfSize = bmInfo_bar.bmiHeader.biSizeImage;
	barHeader.bfType = 0x4D42;
	settHeader.bfOffBits = rawDataOffset;
	settHeader.bfSize = bmInfo_sett.bmiHeader.biSizeImage;
	settHeader.bfType = 0x4D42;
	settoverHeader.bfOffBits = rawDataOffset;
	settoverHeader.bfSize = bmInfo_sett_over.bmiHeader.biSizeImage;
	settoverHeader.bfType = 0x4D42;
	borderPixels = new BYTE[bmInfo_border.bmiHeader.biSizeImage + rawDataOffset];
	innerPixels = new BYTE[bmInfo_inner.bmiHeader.biSizeImage + rawDataOffset];
	closePixels = new BYTE[bmInfo_close.bmiHeader.biSizeImage + rawDataOffset];
	closeoverPixels = new BYTE[bmInfo_close_over.bmiHeader.biSizeImage + rawDataOffset];
	minPixels = new BYTE[bmInfo_min.bmiHeader.biSizeImage + rawDataOffset];
	minoverPixels = new BYTE[bmInfo_min_over.bmiHeader.biSizeImage + rawDataOffset];
	barPixels = new BYTE[bmInfo_bar.bmiHeader.biSizeImage + rawDataOffset];
	settPixels = new BYTE[bmInfo_sett.bmiHeader.biSizeImage + rawDataOffset];
	settoverPixels = new BYTE[bmInfo_sett_over.bmiHeader.biSizeImage + rawDataOffset];
	*(BITMAPFILEHEADER*)borderPixels = borderHeader;
	*(BITMAPINFO*)(borderPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_border;
	*(BITMAPFILEHEADER*)innerPixels = innerHeader;
	*(BITMAPINFO*)(innerPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_inner;
	*(BITMAPFILEHEADER*)closePixels = closeHeader;
	*(BITMAPINFO*)(closePixels + sizeof(BITMAPFILEHEADER)) = bmInfo_close;
	*(BITMAPFILEHEADER*)closeoverPixels = closeoverHeader;
	*(BITMAPINFO*)(closeoverPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_close_over;
	*(BITMAPFILEHEADER*)minPixels = minHeader;
	*(BITMAPINFO*)(minPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_min;
	*(BITMAPFILEHEADER*)minoverPixels = minoverHeader;
	*(BITMAPINFO*)(minoverPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_min_over;
	*(BITMAPFILEHEADER*)barPixels = barHeader;
	*(BITMAPINFO*)(barPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_bar;
	*(BITMAPFILEHEADER*)settPixels = settHeader;
	*(BITMAPINFO*)(settPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_sett;
	*(BITMAPFILEHEADER*)settoverPixels = settoverHeader;
	*(BITMAPINFO*)(settoverPixels + sizeof(BITMAPFILEHEADER)) = bmInfo_sett_over;
	GetDIBits(hdc, bm_border, 0, bmInfo_border.bmiHeader.biHeight, (LPVOID)(borderPixels + rawDataOffset), &bmInfo_border, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_inner, 0, bmInfo_inner.bmiHeader.biHeight, (LPVOID)(innerPixels + rawDataOffset), &bmInfo_inner, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_close, 0, bmInfo_close.bmiHeader.biHeight, (LPVOID)(closePixels + rawDataOffset), &bmInfo_close, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_close_over, 0, bmInfo_close_over.bmiHeader.biHeight, (LPVOID)(closeoverPixels + rawDataOffset), &bmInfo_close_over, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_min, 0, bmInfo_min.bmiHeader.biHeight, (LPVOID)(minPixels + rawDataOffset), &bmInfo_min, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_min_over, 0, bmInfo_min_over.bmiHeader.biHeight, (LPVOID)(minoverPixels + rawDataOffset), &bmInfo_min_over, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_bar, 0, bmInfo_bar.bmiHeader.biHeight, (LPVOID)(barPixels + rawDataOffset), &bmInfo_bar, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_sett, 0, bmInfo_sett.bmiHeader.biHeight, (LPVOID)(settPixels + rawDataOffset), &bmInfo_sett, DIB_RGB_COLORS);
	GetDIBits(hdc, bm_sett_over, 0, bmInfo_sett_over.bmiHeader.biHeight, (LPVOID)(settoverPixels + rawDataOffset), &bmInfo_sett_over, DIB_RGB_COLORS);
	img_border.open(borderPixels, bmInfo_border.bmiHeader.biSizeImage);
	img_inner.open(innerPixels, bmInfo_inner.bmiHeader.biSizeImage);
	img_close_normal.open(closePixels, bmInfo_close.bmiHeader.biSizeImage);
	img_close_down.open(closeoverPixels, bmInfo_close_over.bmiHeader.biSizeImage);
	img_min_normal.open(minPixels, bmInfo_min.bmiHeader.biSizeImage);
	img_min_down.open(minoverPixels, bmInfo_min_over.bmiHeader.biSizeImage);
	img_header.open(barPixels, bmInfo_bar.bmiHeader.biSizeImage);
	img_sett_normal.open(settPixels, bmInfo_sett.bmiHeader.biSizeImage);
	img_sett_down.open(settoverPixels, bmInfo_sett_over.bmiHeader.biSizeImage);
	ReleaseDC(NULL, hdc);
	bg_border.load(img_border, nana::rectangle(0, 0, 590, 340));
	bg_border.stretchable(0, 0, 0, 0);
	bg_border.transparent(true);
	bg_inner.load(img_inner, nana::rectangle(0, 0, 582, 299));
	bg_inner.stretchable(sideBorderWidth, 0, sideBorderWidth, bottomBarHeight);
	bg_inner.transparent(false);

	// TODO: Form code starts here

	// Load X and Y location for the form from the ini file (if exists)
	int posX = m_parent.LoadX();
	int posY = m_parent.LoadY();
	move	(posX, posY);

	// This code sets the plugin size, title and what to do when the X is pressed
	size(nana::size(formWidth, formHeight));
	caption ("SDRuno Plugin rtty");
	events().destroy([&] { m_parent.FormClosed(); });

	//Initialize header bar
	header_bar.size(nana::size(122, 20));
	header_bar.load(img_header, nana::rectangle(0, 0, 122, 20));
	header_bar.stretchable(0, 0, 0, 0);
	header_bar.move(nana::point((formWidth / 2) - 61, 5));
	header_bar.transparent(true);

	//Initial header text 
	title_bar_label.size(nana::size(65, 12));
	title_bar_label.move(nana::point((formWidth / 2) - 5, 9));
	title_bar_label.format(true);
	title_bar_label.caption("< bold size = 6 color = 0x000000 font = \"Verdana\">RTTY decoder</>");
	title_bar_label.text_align(nana::align::center, nana::align_v::center);
	title_bar_label.fgcolor(nana::color_rgb(0x000000));
	title_bar_label.transparent(true);

	//Iniitialize drag_label
	form_drag_label.move(nana::point(0, 0));
	form_drag_label.transparent(true);

	//Initialize dragger and set target to form, and trigger to drag_label 
	form_dragger.target(*this);
	form_dragger.trigger(form_drag_label);

	//Initialise the "Minimize button"
	min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15));
	min_button.bgcolor(nana::color_rgb(0x000000));
	min_button.move(nana::point(formWidth - 51, 9));
	min_button.transparent(true);
	min_button.events().mouse_down([&] { min_button.load(img_min_down, nana::rectangle(0, 0, 20, 15)); });
	min_button.events().mouse_up([&] { min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15)); nana::API::zoom_window(this->handle(), false); });
	min_button.events().mouse_leave([&] { min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15)); });

	//Initialise the "Close button"
	close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15));
	close_button.bgcolor(nana::color_rgb(0x000000));
	close_button.move(nana::point(formWidth - 26, 9));
	close_button.transparent(true);
	close_button.events().mouse_down([&] { close_button.load(img_close_down, nana::rectangle(0, 0, 20, 15)); });
	close_button.events().mouse_up([&] { close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15)); close(); });
	close_button.events().mouse_leave([&] { close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15)); });

	//Uncomment the following block of code to Initialise the "Setting button"
	sett_button.load(img_sett_normal, nana::rectangle(0, 0, 40, 15));
	sett_button.bgcolor(nana::color_rgb(0x000000));
	sett_button.move(nana::point(10, 9));
	sett_button.events().mouse_down([&] { sett_button.load(img_sett_down, nana::rectangle(0, 0, 40, 15)); });
	sett_button.events().mouse_up([&] { sett_button.load(img_sett_normal, nana::rectangle(0, 0, 40, 15)); SettingsButton_Click(); });
	sett_button.events().mouse_leave([&] { sett_button.load(img_sett_normal, nana::rectangle(0, 0, 40, 15)); });
	sett_button.tooltip("Show settings window");
	sett_button.transparent(true);

// TODO: Extra Form code goes here

	strengthMeter.  caption (std::to_string (0));
	strengthMeter. tooltip ("IF after correction");
	freqCorrection. caption (std::to_string (0));
	freqCorrection. tooltip ("estimated freq correction");
	rttyBaudRate.  caption (std::to_string (0));
	rttyBaudRate. tooltip ("estimated baudrate");
	rttyGuess.caption("0");
	rttyGuess. tooltip ("estimated shift");

	xx1.caption ("shift");
	widthSelector. push_back ("170");
	widthSelector. push_back ("85");
        widthSelector. push_back ("300");
        widthSelector. push_back ("450");
        widthSelector. push_back ("600");
        widthSelector. push_back ("850");
        widthSelector. push_back ("900");
        widthSelector. push_back ("1200");
	widthSelector. option (0);
	widthSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	                set_rttyShift (ar_cbx. widget. caption ());});

	
	xx2.caption("rate");
	rateSelector.push_back("45");
        rateSelector. push_back ("50");
        rateSelector. push_back ("75");
        rateSelector. push_back ("100");
        rateSelector. push_back ("300");
	rateSelector. option (0);
	rateSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	                set_rttyBaudrate (ar_cbx. widget. caption ());});

	xx3. caption ("nr bits");
	bitSelector. push_back ("5 bits");
        bitSelector. push_back ("7 bits");
        bitSelector. push_back ("8 bits");
	bitSelector. option (0);
		bitSelector.events().selected([&](const nana:: arg_combox& ar_cbx) {
	                set_rttyNbits (ar_cbx. widget. caption ());});

	xx4. caption ("stopbits");
	stopbitsSelector. push_back ("1 stop");
        stopbitsSelector. push_back ("1.5 stop");
        stopbitsSelector. push_back ("2 stop");
	stopbitsSelector. option (0);
	stopbitsSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	                set_rttyStopbits (ar_cbx. widget. caption ());});

	xx5. caption ("parity");
	paritySelector. push_back ("par none");
        paritySelector. push_back ("par one");
        paritySelector. push_back ("par odd");
        paritySelector. push_back ("par even");
        paritySelector. push_back ("par zero");
	paritySelector. option (0);
	paritySelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	                set_rttyParity (ar_cbx. widget. caption ());});

	xx6. caption ("msb");
	msbSelector. push_back ("msb false");
        msbSelector. push_back ("msb true");
	paritySelector. option (0);
	msbSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	                set_rttyMsb (ar_cbx. widget. caption ());});

	xx7. caption ("afc");
	afcSelector. push_back ("afc off");
	afcSelector. push_back ("afc on");
	afcSelector. option (0);
	afcSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	               set_rttyAfcon (ar_cbx. widget. caption ());});

	xx8. caption ("reversed");
	normalSelector. push_back ("normal");
	normalSelector. push_back ("reverse");
	normalSelector. option (0);
	normalSelector. events(). selected ([&](const nana::arg_combox &ar_cbx){
	              set_rttyReverse (ar_cbx. widget. caption ());});

	rttyText.transparent(true);
	rttyText.fgcolor(nana::colors::white);

	delete[] borderPixels;
	delete[] innerPixels;
	delete[] closePixels;
	delete[] closeoverPixels;
	delete[] minPixels;
	delete[] minoverPixels;
	delete[] barPixels;
	delete[] barfocusedPixels;
	delete[] settPixels;
	delete[] settoverPixels;
}

void SDRunoPlugin_rttyForm::SettingsButton_Click () {
	// TODO: Insert code here to show settings panel
}

