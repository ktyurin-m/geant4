#include <iostream>
#include <fstream>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>


void analysis(){
// string cwa = "check_with_air.root";
Double_t a = 2;
Double_t b = 4;
    TFile *f = new TFile("out.root");
    TTree *t = (TTree*)f->Get("Data");
    Double_t z;
    Double_t y;
    Double_t x;
    Double_t Energy;
    t->SetBranchAddress("y",&y);
    t->SetBranchAddress("z",&z);
    t->SetBranchAddress("x",&x);
    t->SetBranchAddress("Energy",&Energy);

    TH2F *h1 = new TH2F("h1","h1",100,-60,60,100,0,1800);
    TH2F *h2 = new TH2F("h2","h2",100,-60,60,100,-60,60);
    TH1F *h3  = new TH1F("HistOfEnergy","HistOfEnergy",200, 0,1800) ; //energy
    // TH1F *h4  = new TH1F("h3","h3",300, -100,100) ;
    Int_t ent = t->GetEntries();
    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);
        if (x == 0)
        {   
            h1->Fill(z, Energy);
            h3->Fill(Energy);
            h2->Fill(y,z);
        }
        

        // h4->Fill(x);
        // h2->Fill(x,Energy);
        // h1->Fill(x,y);
    }

    t->ResetBranchAddresses();
    TCanvas* c = new TCanvas;
    h3->GetYaxis()->SetTitle("Event");
    h3->GetXaxis()->SetTitle("Energy, MeV");
	h3->SetTitle("Energy");
    // h3->Draw();

    // h2->Draw("colz");
        h2->Draw("colz");
    c->Update();
}
