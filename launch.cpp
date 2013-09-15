//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "launch.h"
#include "main.h"
#include <psapi.h>
#include <dir.h>
#include <Userenv.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
game *game;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	if (Edit2->Text!="" && Edit3->Text!="")
	{
		game->CreateProc(Edit2->Text.c_str(), OpenDialog1->FileName.c_str());
	}else{
		OpenDialog1->Execute();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	game->WriteINI("general", "inject_name", Edit1->Text.t_str(), "./settings.set");
	game->WriteINI("general", "ko_name", Edit2->Text.t_str(), "./settings.set");
	game->WriteINI("general", "ko_adress", Edit3->Text.t_str(), "./settings.set");

	DeleteFile("D:\\Mgame\\KnightOnlineEn - 2\\log.klg");
	DeleteFile("D:\\Mgame\\KnightOnlineEn - 2\\Scheduler.ini");
	Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	game->yukle();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenDialog1CanClose(TObject *Sender, bool &CanClose)
{
	Edit3->Text=OpenDialog1->Files->GetText();

	if (Edit2->Text!="" && Edit3->Text!="")
		game->CreateProc(OpenDialog1->FileName.c_str(), OpenDialog1->FileName.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	bool dosya, klasor;
	char *buffer;

	klasor=DirectoryExists("AppData\\Local\\deb bot");
	buffer=getenv("USERPROFILE");

	ChDir(buffer);

	if (klasor==true)
	{
		ChDir("AppData\\Local\\deb bot");
	}else{
		MkDir("AppData\\Local\\deb bot");
		ChDir("AppData\\Local\\deb bot");
	}

		Edit1->Text=game->ReadINI("general", "inject_name", "./settings.set", Edit1->Text.c_str());
		Edit2->Text=game->ReadINI("general", "ko_name", "./settings.set", Edit2->Text.c_str());
		Edit3->Text=game->ReadINI("general", "ko_adress", "./settings.set", Edit3->Text.c_str());

	OpenDialog1->FileName=Edit3->Text.c_str();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
	Edit2->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
	Edit2->Enabled=false;

	srand(time(NULL));
	Edit2->Text=rand();
}
//---------------------------------------------------------------------------
