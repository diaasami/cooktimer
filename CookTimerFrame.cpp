// -*- C++ -*- generated by wxGlade 0.5 on Wed May 30 14:46:15 2007 from Z:\WIP\CookTimer\CookTimer.wxg

#include "CookTimerFrame.h"

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
	#include "A.xpm"
	#include "TrayIcon.xpm"
#endif

BEGIN_EVENT_TABLE(CookTimerTaskBarIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_DCLICK  (CookTimerTaskBarIcon::OnLeftButtonDClick)
END_EVENT_TABLE()

void CookTimerTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent& evt)
	{
	const bool WasShown = window->IsShown();
	
	if(!WasShown && window->IsIconized())
		{
			window->Iconize(WasShown);
			window->Show(!WasShown);
		}
		
	window->Raise();
	}

BEGIN_EVENT_TABLE(CookTimerFrame, wxFrame)
	EVT_TIMER(wxID_ANY, CookTimerFrame::OnTimer)
	EVT_BUTTON(ID_StartStopButton, CookTimerFrame::OnStartStopButton)
	EVT_BUTTON(ID_ResetButton, CookTimerFrame::OnResetButton)
	EVT_ICONIZE(CookTimerFrame::OnIconize)
	EVT_RADIOBOX(wxID_ANY, CookTimerFrame::OnPresetsRadioBox)
	EVT_CHECKBOX(ID_AutoRestartCheckBox, CookTimerFrame::OnAutoResetChecked)
END_EVENT_TABLE()

CookTimerFrame::CookTimerFrame(wxWindow* parent)
	: wxFrame(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxTAB_TRAVERSAL | wxCLIP_CHILDREN),
	  _timer(this, wxID_ANY),
	  _seconds(0),
	  _running(false),
	  _reset(true),
	#ifdef _WIN32
	  timeoutSound(wxT("Bell"), true)
	#else
	  timeoutSound(wxT("alarm.wav"), false)
	#endif	// _WIN32
 	{
	#ifdef _WIN32
		// The icon in the resources contains both sizes and is sufficient on
		// its own
		SetIcon(wxICON(A));
	#else
		SetIcons(wxICON(TrayIcon));
	#endif	// !_WIN32
	
	const wxString presetsRadioBox_choices[] = {
		_("3 min"),
		_("5 min"),
		_("10 min"),
		_("15 min"),
		_("Custom"),
	};
	const int times[] = {3 * 60, 5 * 60, 10 * 60, 15 * 60, 0};
#ifdef _WIN32	// doesn't work on linux, gives a compiler error don't know why :(
	wxCOMPILE_TIME_ASSERT(WXSIZEOF(presetsRadioBox_choices) == WXSIZEOF(times), times);
#endif	// _WIN32

	_times.assign(times, times + WXSIZEOF(times));
	_presetsRadioBox = new wxRadioBox(this, wxID_ANY, _("Presets"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(times), presetsRadioBox_choices, 1, wxRA_SPECIFY_ROWS);
	
	// begin wxGlade: CookTimerFrame::CookTimerFrame
	rootPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxCLIP_CHILDREN);
	_hoursSpinCtrl = new wxSpinCtrl(rootPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
	_minutesSpinCtrl = new wxSpinCtrl(rootPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 60);
	_secondsSpinCtrl = new wxSpinCtrl(rootPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 60);
	_remainingTimeStatic = new wxStaticText(rootPanel, wxID_ANY, wxEmptyString);
	_progressBar = new wxGauge(rootPanel, wxID_ANY, 10, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH);
	_startStopButton = new wxButton(rootPanel, ID_StartStopButton, wxEmptyString);
	_resetButton = new wxButton(rootPanel, ID_ResetButton, _("&Reset"));
	_autoRestartCheckbox = new wxCheckBox(rootPanel, ID_AutoRestartCheckBox, _("Automatic restart"));
	_ringForeverCheckbox = new wxCheckBox(rootPanel, wxID_ANY, _("Ring forever"));

	set_properties();
	do_layout();
	// end wxGlade

	// Make sibling of other controls to fix tab order.
	_presetsRadioBox->Reparent(rootPanel);

	_presetsRadioBox->SetFocus();
	
	_taskBarIcon = new CookTimerTaskBarIcon();
#if defined(__WXCOCOA__)
	_dockIcon = new CookTimerTaskBarIcon(wxTaskBarIcon::DOCK);
#endif
	if (!_taskBarIcon->SetIcon(wxICON(TrayIcon), wxT("CookTimer")))
		wxMessageBox(wxT("Could not set icon."));

	_taskBarIcon->window = this;

	UpdateControls();

	rootPanel->Layout();
	}

CookTimerFrame::~CookTimerFrame()
	{
	delete _taskBarIcon;
#if defined(__WXCOCOA__)
	delete _dockIcon;
#endif
	}

void CookTimerFrame::set_properties()
	{
	// begin wxGlade: CookTimerFrame::set_properties
	SetTitle(_("Cook Timer"));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	_hoursSpinCtrl->SetMinSize(wxSize(60, -1));
	_hoursSpinCtrl->SetToolTip(_("Hours"));
	_minutesSpinCtrl->SetMinSize(wxSize(60, -1));
	_minutesSpinCtrl->SetToolTip(_("Minutes"));
	_secondsSpinCtrl->SetMinSize(wxSize(60, -1));
	_secondsSpinCtrl->SetToolTip(_("Seconds"));
	_remainingTimeStatic->SetFont(wxFont(32, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
	_startStopButton->SetDefault();
	_autoRestartCheckbox->SetToolTip(_("Restart countdown automatically when it reaches zero"));
	// end wxGlade
	
	_presetsRadioBox->SetSelection(0);
	}

void CookTimerFrame::do_layout()
	{
	// begin wxGlade: CookTimerFrame::do_layout
	wxBoxSizer* rootSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_4 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxStaticText* label_1 = new wxStaticText(rootPanel, wxID_ANY, _("Hours"));
	sizer_1->Add(label_1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	sizer_1->Add(_hoursSpinCtrl, 0, wxALL, 3);
	sizer_3->Add(sizer_1, 1, wxEXPAND, 0);
	wxStaticText* label_2 = new wxStaticText(rootPanel, wxID_ANY, _("Minutes"));
	sizer_2->Add(label_2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	sizer_2->Add(_minutesSpinCtrl, 0, wxALL, 3);
	sizer_3->Add(sizer_2, 1, wxEXPAND, 0);
	wxStaticText* label_3 = new wxStaticText(rootPanel, wxID_ANY, _("Seconds"));
	sizer_4->Add(label_3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	sizer_4->Add(_secondsSpinCtrl, 0, wxALL, 3);
	sizer_3->Add(sizer_4, 1, wxEXPAND, 0);
	leftSizer->Add(sizer_3, 1, wxALIGN_CENTER_HORIZONTAL, 0);
	leftSizer->Add(_remainingTimeStatic, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
	leftSizer->Add(_progressBar, 1, wxALL|wxEXPAND, 3);
	panelSizer->Add(leftSizer, 1, wxEXPAND, 0);
	wxStaticLine* static_line_1 = new wxStaticLine(rootPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	panelSizer->Add(static_line_1, 0, wxEXPAND, 0);
	buttonsSizer->Add(_startStopButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
	buttonsSizer->Add(_resetButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
	buttonsSizer->Add(_autoRestartCheckbox, 0, wxALL, 3);
	buttonsSizer->Add(_ringForeverCheckbox, 0, wxALL, 3);
	panelSizer->Add(buttonsSizer, 0, wxALIGN_CENTER_VERTICAL, 0);
	rootPanel->SetSizer(panelSizer);
	rootSizer->Add(rootPanel, 1, wxEXPAND, 0);
	SetSizer(rootSizer);
	rootSizer->Fit(this);
	Layout();
	Centre();
	// end wxGlade

	leftSizer->Prepend(_presetsRadioBox, 0, wxALL, 3);
	rootSizer->Fit(this);
	Layout();
	Centre();
	}

void CookTimerFrame::OnTimer(wxTimerEvent &event)
	{
	_seconds --;

	if (_seconds == 0)
		{
		if(!IsShown() && IsIconized())
			{
			Iconize(false);
			}

		Raise();

		if (_autoRestartCheckbox->IsChecked())
			{
			timeoutSound.Play(wxSOUND_ASYNC);
			_seconds = GetPeriod();
			
			if (_seconds == 0)	// check for invalid value
				{
				wxBell();
				
				_timer.Stop();
				
				_running = false;
				_reset = true;
				}
			}
		else
			{
			timeoutSound.Play((_ringForeverCheckbox->GetValue() ? wxSOUND_LOOP : 0) | wxSOUND_ASYNC);
			_timer.Stop();
			
			_running = false;
			_reset = true;
			}

		_taskBarIcon->SetIcon(wxICON(TrayIcon), wxT("CookTimer"));
		}

	UpdateControls();
	}

void CookTimerFrame::OnStartStopButton(wxCommandEvent &evt)
	{
	StartStopTimer();
	}

void CookTimerFrame::OnResetButton(wxCommandEvent &evt)
	{
	timeoutSound.Stop();

	if (_running)
		{
		wxCommandEvent dummy;
		OnStartStopButton(dummy);
		}

	_reset = true;
	_seconds = 0;

	UpdateControls();
	}

void CookTimerFrame::UpdateControls()
	{
	_presetsRadioBox->Enable(_reset);
	_autoRestartCheckbox->Enable(_reset);
	_ringForeverCheckbox->Enable(_reset);
	
	_hoursSpinCtrl->Enable(_reset && IsCustomSelected());
	_minutesSpinCtrl->Enable(_reset && IsCustomSelected());
	_secondsSpinCtrl->Enable(_reset && IsCustomSelected());

	_startStopButton->SetLabel(_running ? _("&Stop") : _("&Start"));

		{
		const unsigned int hours = _seconds / 3600;
		const unsigned int minutes = _seconds % 3600 / 60;
		const unsigned int seconds = _seconds % 60;

		_remainingTimeStatic->SetLabel(wxString::Format(wxT("%02d:%02d:%02d"), hours, minutes, seconds));
		Layout();

		wxString taskbarIconTooltip(wxT("Cook Timer"));
		if (_running)
			taskbarIconTooltip += wxString::Format(wxT("- %02d:%02d:%02d"), hours, minutes, seconds); 

		_taskBarIcon->SetIcon(wxICON(TrayIcon), taskbarIconTooltip);
		
		this->SetTitle(wxString::Format(wxT("%02d:%02d:%02d - Cook Timer"), hours, minutes, seconds));
		}

	_progressBar->SetValue(_seconds);
	}

void CookTimerFrame::OnIconize(wxIconizeEvent &evt)
	{
	if (evt.IsIconized())
		{ 				// if the event reports that the frame has just been iconized
		Show(false);	// removes minimized window from taskbar, leaving only systray icon
		}
	}

unsigned int CookTimerFrame::GetCustomValue() const
	{
	unsigned int hours, minutes, seconds;

	hours = _hoursSpinCtrl->GetValue();
	minutes = _minutesSpinCtrl->GetValue();
	seconds = _secondsSpinCtrl->GetValue();

	return hours * 3600 + minutes * 60 + seconds;
	}

bool CookTimerFrame::IsCustomSelected() const
	{
	const unsigned int selectedIdx = _presetsRadioBox->GetSelection();

	return _times[selectedIdx] == 0;
	}

unsigned int CookTimerFrame::GetPeriod() const
	{
	wxASSERT(_presetsRadioBox->GetSelection() != -1);

	if (!IsCustomSelected())
		{
		const unsigned int selectedIdx = _presetsRadioBox->GetSelection();
		return _times[selectedIdx];
		}
	else
		{
		return GetCustomValue();
		}		
	}

void CookTimerFrame::OnAutoResetChecked(wxCommandEvent &evt)
	{
	if (evt.IsChecked())
		_ringForeverCheckbox->SetValue(false);

	_ringForeverCheckbox->Enable(!evt.IsChecked());
	}

void CookTimerFrame::StartStopTimer()
	{
	if (_running)
		{
		_timer.Stop();
		_reset = false;
		_running = false;
		}
	else
		{
		if (_reset)
			{
			_seconds = GetPeriod();
			
			if (_seconds == 0)	// check for invalid value
				{
				wxBell();
				return;
				}
			
			_progressBar->SetRange(_seconds);
			}

		timeoutSound.Stop();
		_startStopButton->SetFocus();
		_timer.Start(1000);
		
		_reset = false;
		_running = true;
		}

	UpdateControls();
	}

void CookTimerFrame::SetTime(unsigned long inSeconds, bool start)
	{
	assert(inSeconds != 0);

	const unsigned int hours = inSeconds / 3600;
	const unsigned int minutes = inSeconds % 3600 / 60;
	const unsigned int seconds = inSeconds % 60;

	_hoursSpinCtrl->SetValue(hours);
	_minutesSpinCtrl->SetValue(minutes);
	_secondsSpinCtrl->SetValue(seconds);

	_presetsRadioBox->SetSelection(4);

	UpdateControls();

	if (start)
		{
		StartStopTimer();
		}
	}
