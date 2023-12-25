void drawGraphFromDataFile() {
TGraph *graph = new TGraph("data", "%lg %lg"); // создание графика и загрузка данных из файла data.txt
graph->Draw("APL"); // рисование графика
TLatex latex;

graph->GetXaxis()->SetTitle("#sigma, mm"); // название оси X
graph->GetYaxis()->SetTitle("E_{1}, MeV"); // название оси Y
graph->SetMarkerStyle(20);
gPad->SetGrid(); // включение сетки
gPad->Modified(); // обновление рисунка
gPad->Update(); // обновление рисунка
}