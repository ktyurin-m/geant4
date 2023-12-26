void drawGraphFromDataFile() {
TGraph *graph = new TGraph("data1", "%lg %lg"); // создание графика и загрузка данных из файла data.txt
TGraph *graph2 = new TGraph("data2", "%lg %lg");
graph->Draw("APL"); // рисование графика
// graph2->Draw("same");
TLatex latex;

graph->GetYaxis()->SetTitle("std dev, mm"); // название оси X
graph->GetXaxis()->SetTitle("E_{#gamma}, MeV"); // название оси Y
graph->SetMarkerStyle(20);
gPad->SetGrid(); // включение сетки
gPad->Modified(); // обновление рисунка
gPad->Update(); // обновление рисунка
}