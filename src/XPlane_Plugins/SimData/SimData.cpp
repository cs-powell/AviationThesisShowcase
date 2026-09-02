// Downloaded from https://developer.x-plane.com/code-sample/simdata/


/*
 * SimData.c
 * 
 * This example demonstrates how to interact with X-Plane by reading and writing
 * data.  This example creates menus items that change the nav-1 radio frequency.
 * 
 */

#include <stdio.h>
#include <string.h>
// #include "XPLM/XPLMDataAccess.h"
// #include "XPLM/XPLMMenus.h"
#include <stdint.h>   // ADD THIS
#include <iostream>  // ADD THIS
// #include "XPLM/XPLMDataAccess.h"
// #include "XPLM/XPLMUtilities.h"    // Contains XPLMAppendMenuItem
// #include "XPLM/XPLMMenus.h"
// #include "XPLM/XPLMPlugin.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>


#include "SDK/CHeaders/XPLM/XPLMCamera.h"
#include "SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "SDK/CHeaders/XPLM/XPLMDefs.h"
#include "SDK/CHeaders/XPLM/XPLMDisplay.h"
#include "SDK/CHeaders/XPLM/XPLMGraphics.h"
#include "SDK/CHeaders/XPLM/XPLMMenus.h"
#include "SDK/CHeaders/XPLM/XPLMNavigation.h"
#include "SDK/CHeaders/XPLM/XPLMPlanes.h"
#include "SDK/CHeaders/XPLM/XPLMPlugin.h"
#include "SDK/CHeaders/XPLM/XPLMProcessing.h"
// #include "SDK/CHeaders/XPLM/XPLMScenery.h"
#include "SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "SDK/CHeaders/XPLM/XPLMUtilities.h"
#include <map>

/* We keep our data ref globally since only one is used for the whole plugin. */
static XPLMDataRef gDataRef = NULL;
static XPLMDataRef pitch = NULL;
static XPLMDataRef roll = NULL;
static XPLMDataRef heading = NULL;
static XPLMFlightLoopID loopID = NULL;
static std::ofstream myfile;
static std::string presignedURL = "https://aviationthesisdatatest.s3.us-east-1.amazonaws.com/Data.txt?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIA47CRUI57V7N27UB6%2F20251204%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20251204T200119Z&X-Amz-Expires=3600&X-Amz-SignedHeaders=host&X-Amz-Signature=a887998178fba3c406b13f4e3312d33b5a8c4e690a68e853b5e278f90a3e37ab";


float write(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void *inRefcon){
		float array[3] = {XPLMGetDataf(pitch), XPLMGetDataf(roll), XPLMGetDataf(heading)};
		// std::ofstream myfile;
		myfile << inElapsedSinceLastCall << "," << array[0] <<"," << array[1] <<"," << array[2] << "\n";
		myfile.flush();
		// XPLMDebugString("Nav1 frequency changed.\n");
		return -1.0;
	}

static void	MyMenuHandlerCallback(void *               inMenuRef,    void *               inItemRef);    
PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc){


		XPLMMenuID	myMenu;
		int			mySubMenuItem;

	/* Provide our plugin's profile to the plugin system. */
	strcpy(outName, "SimData");
	strcpy(outSig, "xplanesdk.examples.simdata");
	strcpy(outDesc, "A plugin that changes sim data.");

	/* First we put a new menu item into the plugin menu.
	 * This menu item will contain a submenu for us. */
	mySubMenuItem = XPLMAppendMenuItem(
						XPLMFindPluginsMenu(),	/* Put in plugins menu */
						"Sim Data",				/* Item Title */
						0,						/* Item Ref */
						1);						/* Force English */
	
	/* Now create a submenu attached to our menu item. */
	myMenu = XPLMCreateMenu(
						"Sim Data", 
						XPLMFindPluginsMenu(), 
						mySubMenuItem, 			/* Menu Item to attach to. */
						MyMenuHandlerCallback,	/* The handler */
						0);						/* Handler Ref */
						
	/* Append a few menu items to our submenu.  We will use the refcon to
	 * store the amount we want to change the radio by. */
    XPLMAppendMenuItem(
        myMenu,
        "Decrement Nav1",
        (void*)(intptr_t)-1000,
        1);

    XPLMAppendMenuItem(
        myMenu,
        "Increment Nav1",
        (void*)(intptr_t)+1000,
        1);

	XPLMAppendMenuItem(
	myMenu,
	"Log to Datafile",
	(void*)(intptr_t)+1000,
	1);


	XPLMAppendMenuItem(
	myMenu,
	"Upload Datafile",
	(void*)(intptr_t)+1000,
	2);

	/* Look up our data ref.  You find the string name of the data ref
	 * in the master list of data refs, including in HTML form in the 
	 * plugin SDK.  In this case, we want the nav1 frequency. */
	gDataRef = XPLMFindDataRef("sim/cockpit/radios/nav1_freq_hz");

	pitch = XPLMFindDataRef("sim/flightmodel/position/theta"); // pitch
	roll = XPLMFindDataRef("sim/flightmodel/position/phi");   // roll
	heading = XPLMFindDataRef("sim/flightmodel/position/psi");   // heading
	myfile.open ("/Users/flyingtopher/Applications/X-Plane 11/Data.txt");
	myfile << "Information About the Experiment Subject\n";
	myfile << "Data About their Machine, version, aircraft, device, etc.\n";
	myfile << "Experimental Data Begins Below:\n";
	myfile << "<sep>\n";
	myfile << "Pitch,Roll,Heading\n";
	myfile.flush();

	/* Only return that we initialized correctly if we found the data ref. */
	return (gDataRef != NULL) ? 1 : 0;
}

PLUGIN_API void	XPluginStop(void)
{
	myfile.close();
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
}

bool uploadToS3(
    const std::string& presignedUrl,
    const std::map<std::string, std::string>& fields,
    const std::string& filePath)
{
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    struct curl_httppost* form = nullptr;
    struct curl_httppost* last = nullptr;

    // Add all required S3 POST fields
    for (const auto& kv : fields) {
        curl_formadd(&form, &last,
            CURLFORM_COPYNAME, kv.first.c_str(),
            CURLFORM_COPYCONTENTS, kv.second.c_str(),
            CURLFORM_END);
    }

    // Attach the file itself
    curl_formadd(&form, &last,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, filePath.c_str(),
        CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_URL, presignedUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "XPlanePluginUploader/1.0");

    // Optional: capture response
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
        [](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
            std::string* resp = (std::string*)userdata;
            resp->append(ptr, size * nmemb);
            return size * nmemb;
        }
    );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_formfree(form);

    return (res == CURLE_OK);
}

void MyMenuHandlerCallback(void *inMenuRef, void *inItemRef)
{
    if (gDataRef != NULL) {
        int delta = (int)(intptr_t)inItemRef;   // SAFE CONVERSION
        XPLMSetDatai(gDataRef, XPLMGetDatai(gDataRef) + delta);
		// std::cout << "Nav1 frequency changed by " << delta << " Hz." << std::endl;
		//  XPLMDebugString("Nav1 frequency changed.");
		// float array[3] = {XPLMGetDataf(pitch), XPLMGetDataf(roll), XPLMGetDataf(heading)};
		// // std::ofstream myfile;
		// myfile << array[0] <<"," << array[1] <<"," << array[2] << "\n";
		// myfile.flush();
		XPLMDebugString("Entered menu callback\n");
		XPLMCreateFlightLoop_t params = {0};
		params.structSize = sizeof(XPLMCreateFlightLoop_t);
		params.phase = xplm_FlightLoop_Phase_BeforeFlightModel;
		params.callbackFunc = write;
		params.refcon = nullptr;
		loopID = XPLMCreateFlightLoop(&params);
		XPLMScheduleFlightLoop(loopID, 1, 1);
    }

	//AWS METHOD:
// 	std::map<std::string, std::string> fields = {
//     {"key", "logs/Data.txt"},
//     {"policy", "base64policy"},
//     {"x-amz-algorithm", "AWS4-HMAC-SHA256"},
//     {"x-amz-credential", "...."},
//     {"x-amz-date", "20250104T000000Z"},
//     {"x-amz-signature", "...."}
// };

// bool ok = uploadToS3(
//     "https://mybucket.s3.amazonaws.com/",
//     fields,
//     "/Users/flyingtopher/Applications/X-Plane 11/Data.txt"
// );

	//CURL METHOD: 	
		// XPLMDebugString("Uploading to AWS\n");
		// const char* url = presignedURL.c_str();

		// CURL* curl = curl_easy_init();
		// if (!curl) {
		// 	XPLMDebugString("Failed to Upload to AWS\n");
		// 	// std::cerr << "Failed to initialize CURL\n";
		// 	return;
		// } else {
		// 	XPLMDebugString("CURL Initialized\n");
		// };

		// curl_easy_setopt(curl, CURLOPT_URL, url);

		// FILE* file = fopen("/Users/flyingtopher/Applications/X-Plane 11/Data.txt", "rb");
		// if(file) {
		// 	XPLMDebugString("File opened successfully\n");
		// } else {
		// 	XPLMDebugString("Failed to open file\n");
		// 	return;
		// }
		// curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		// curl_easy_setopt(curl, CURLOPT_READDATA, file);

		// CURLcode res = curl_easy_perform(curl);

		// if (res != CURLE_OK){
		// 	XPLMDebugString("Upload Fail\n");
		// 	std::cerr << curl_easy_strerror(res) << "\n";
		// } else {
		// 	XPLMDebugString("Upload Success\n");
		// 	std::cout << "Upload OK\n";
		// }
		// curl_easy_cleanup(curl);
		// fclose(file);
}