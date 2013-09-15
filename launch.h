//---------------------------------------------------------------------------

#ifndef launchH
#define launchH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include "main.h"
#include <ExtCtrls.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TEdit *Edit1;
	TButton *Button1;
	TOpenDialog *OpenDialog1;
	TEdit *Edit2;
	TButton *Button2;
	TLabel *Label1;
	TEdit *Edit3;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton1;
	TLabel *Label2;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall OpenDialog1CanClose(TObject *Sender, bool &CanClose);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RadioButton1Click(TObject *Sender);
	void __fastcall RadioButton2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
