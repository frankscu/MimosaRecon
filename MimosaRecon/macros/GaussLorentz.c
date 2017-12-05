void GaussLorentz(){
  gStyle->SetPalette(1);
  double um=1e-3;

  double sigma = 13.2*um;
  double N0=0.458;
  double N1=6.45*um;
  double d0=-3.98*um;
  double d1=1.80*um;
  double Gamma=3.99*um;

  /*
  double sigma = 17.5*um;
  double N0=0.117;
  double N1=3.71*um;
  double d0=-1.07*um;
  double d1=-4.64*um;
  double Gamma=47.1*um;
  */

  const int nP=420;
  double pixPitch=24*um;
  double dstep=5*pixPitch/nP;
  int kP=0;
  double cdf=0;
  double d,dx,dy,pdf1,pdf2,pdf;
  
  TGraph2D *grPdf = new TGraph2D(nP*nP);
  grPdf->SetNpx(nP);
  grPdf->SetNpy(nP);

  //double factor=1;
  double factor=1/0.553082;
  //double factor=1/0.67009;

  for(int i=0;i<nP;i++){
    dx=-pixPitch/2+i*dstep;
    for(int j=0;j<nP;j++){
      dy=-pixPitch/2+j*dstep;
      d=TMath::Sqrt(dx*dx+dy*dy);
      pdf1=exp(-pow((d-d0),2)/(2*sigma*sigma));
      pdf2=Gamma/(Gamma*Gamma+pow((d-d1),2));
      pdf = factor*(N0*pdf1+N1*pdf2)*dstep*dstep;
      grPdf->SetPoint(kP,dx,dy,pdf);
      cdf += pdf ;
      kP++;
      if(kP%1000==0)cout<<"P: "<<kP<<endl;
    }
  }
  TCanvas* c1 = new TCanvas("c1","c1",800,600);
  //grPdf->Draw("surf1");
  grPdf->Draw("colz");
  std::cout<<"cdf:     "<<cdf<<std::endl;

  double r[420];
  double pdfN[420];
  //factor=1./2.25;
  factor=1.;
  for(int i=0;i<nP;i++){
      double _r=i*dstep;
      if(_r>pixPitch){
        sigma = 17.5*um;
        N0=0.117;
        N1=3.71*um;
        d0=-1.07*um;
        d1=-4.64*um;
        Gamma=47.1*um;
      }
      r[i]=_r;
      pdf1=exp(-pow((_r-d0),2)/(2*sigma*sigma));
      pdf2=Gamma/(Gamma*Gamma+pow((_r-d1),2));
      pdfN[i] = factor*(N0*pdf1+N1*pdf2);
  }

  TGraph* gr = new TGraph(nP,r,pdfN);

  TCanvas* c2 = new TCanvas("c2","c2",800,600);
  gr->Draw("APL");
}
