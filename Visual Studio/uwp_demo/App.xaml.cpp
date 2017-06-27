﻿//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <shlobj.h>

#include <realm.hpp>
#include <realm/history.hpp>
#include <realm/lang_bind_helper.hpp>
#include <0.98.0_alltypes_default_encrypted.realm.h>


#include <assert.h>

using namespace App1;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Storage;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}



/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
	using namespace realm;
	using namespace std;


	char key[64];
	for (size_t i = 0; i < 64; ++i) {
		key[i] = 1;
	}

	for (int i = 0; i < 100; i++) {


		//	StorageFolder^ storageFolder2 = ApplicationData::Current->LocalFolder;
		//	concurrency::create_task(storageFolder2->CreateFileAsync("sample.txt", CreationCollisionOption::ReplaceExisting));




		StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
		Platform::String^ path = storageFolder->Path + "\\hang.realm";

		Platform::String^ fooRT = path;
		std::wstring fooW(fooRT->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		const char* pathC = fooA.c_str();



		File f;
		remove(pathC);
		f.open(pathC, realm::util::File::access_ReadWrite, realm::util::File::create_Must, 0);
		f.write((const char*)realmfile, 8192);
		f.close();

		auto hist = make_in_realm_history(pathC);
		SharedGroup sg(*hist, SharedGroupOptions(key));

		Group& group = const_cast<Group&>(sg.begin_read());

		LangBindHelper::advance_read(sg);

		{
			assert(group.size() > 0);
		}

	}



	/*
	{
	Table t;
	t.add_column(type_Int, "ints");
	t.add_empty_row();
	t.set_int(0, 0, 123);
	int64_t i = t.get_int(0, 0);
	}
	*/


	// For ARM, only Release mode is currently configured. Build Core in ARM Release mode too.

	// Enable when you have specified a valid tempdir for the target machine, else it will crash
	/*
	{
	SharedGroupOptions sgo = SharedGroupOptions(SharedGroupOptions::Durability::Full, nullptr, false, std::function<void(int, int)>(), "tempdir");
	SharedGroup sg_w("test.realm", false, sgo);
	WriteTransaction wt(sg_w);
	TableRef t = wt.add_table("table");
	size_t col_ndx = t->add_column(type_String, "column");
	t->add_empty_row(1);
	t->set_string(col_ndx, 0, StringData("string"));
	}
	*/

#if _DEBUG
	// Show graphics profiling information while debugging.
	if (IsDebuggerPresent())
	{
		// Display the current frame rate counters
		DebugSettings->EnableFrameRateCounter = true;
	}
#endif
	auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

	// Do not repeat app initialization when the Window already has content,
	// just ensure that the window is active
	if (rootFrame == nullptr)
	{
		// Create a Frame to act as the navigation context and associate it with
		// a SuspensionManager key
		rootFrame = ref new Frame();

		rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

		if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
		{
			// TODO: Restore the saved session state only when appropriate, scheduling the
			// final launch steps after the restore is complete

		}

		if (e->PrelaunchActivated == false)
		{
			if (rootFrame->Content == nullptr)
			{
				// When the navigation stack isn't restored navigate to the first page,
				// configuring the new page by passing required information as a navigation
				// parameter
				rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
			}
			// Place the frame in the current Window
			Window::Current->Content = rootFrame;
			// Ensure the current window is active
			Window::Current->Activate();
		}
	}
	else
	{
		if (e->PrelaunchActivated == false)
		{
			if (rootFrame->Content == nullptr)
			{
				// When the navigation stack isn't restored navigate to the first page,
				// configuring the new page by passing required information as a navigation
				// parameter
				rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
			}
			// Ensure the current window is active
			Window::Current->Activate();
		}
	}
}




/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void) sender;  // Unused parameter
    (void) e;   // Unused parameter

    //TODO: Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}