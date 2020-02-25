
void WEBsetup()
{
    // pages

  server.on("/", handleRoot);
  server.on("/rgb", handleRGBselect);
  server.on("/setting", handleSetting);
  server.on("/setting_slider", handleSettingSlider);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");


}


void handleRoot()
{
  handleRGBselect();
}

void handleRGBselect()
{
  String message = "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                   "<title>in ukr[ai]ne we trust</title></head><body>"

                   "<h2>AI socket interface</h2>"
                   "by <a href=\"http://www.aiworker.com/\">aiworker</a><br>"
                   "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAHgAAACCCAMAAAC6s/yfAAAAMFBMVEX///8JCf//CQkJ/wm9vf//xMTE/8RFR/7/jo6P/4/v4vP/RkZF/0V/f//q/+pQlkGnUwC5AAACkElEQVR4Ae3Z0W7jIBCFYQawgRin7/+2K49arYQ0/qOSaXclzkUuYqlfD+ZcJaz8RlZWVupvwfFfhxe84AUveMELbr8FpwX/1/AjcpJ+Pt4s1yNAtPFR33+uW2B4c5g2V04ehfUUEa6Hg8vHmOB1uFVODoU1D5hKgudu9ytVD5QntSUXkysfA3z+0P2qIyw/NKm4AexUuR4BYJdJ6fcMz9+vZ+KIfj7fPKnWA0Qb9/b+yjvDYU/Pt98vrixG4dlJ9YZw68FhUniMoq/DYVJUWbSwx6SgssBzt0kJnIjXpHb5XuGtTk6qyzc3s81NqnVxcHlSz7SLg8uTaj2Ig0uT0u9lwt0qTGr76NpfXs2DXG6ulY+adFJnCa+knNOuohf+OSnJgZNl3tXnj7h93a9cAqfkaVdzxOPvpApVtv+5euvWaOWj4TGG29cRRxcTv6aDF+f2AkZyDTi0z0nZU7l/HtE14CdNCqYU0TVgmhRNKV45DHdvNzBPim99NNy038F7oj+OO4/ojrCm06QkEwyuAdOkzhIIZneEcVL6PcJXquHuzYC/JrV/FJgSxHDTPsJD5d4kw5Qm3RFW9MJzgSnNuyO8Jz3ukmFK0+4Ih566HitOCdJu3ZaslJOmBEmGC79J6HRgSgCTa8DacGZKCV0DDpKnppTQteBcpqaUrnTDzXp+dYQHzJ4S64Y7LGSAs+CUGGZ3hPVCwZQYBteAdUIwJYDBNWBtylOiG9YMN58mHCTjlDiWK9mGc+EpTbgmrGgufq4NZ3lI9nNtOBR5T+Hz1j3ltXzjlgv1DU6/SQi4fjC4fjC4fvCVcueGNgnzDYN4wOD6weD6weD6wVdOb5iz4AU7ZcELPsNrWVlZWVlZWfkDlvQS5+WAX9gAAAAASUVORK5CYII='>"

                   "<form id=\"pickForm\" action=\"setting\">"
                   "Select a color:<br>"

                   "<input type=\"color\" name=\"favcolor\" value=\"#";
  message += rgbstring;
  message += "\" onchange=\"mysubmit()\"><br>"

             "<br><input type=\"submit\" value=\"Submit\"></form><br><br>"

             "<form id=\"slideForm\" action=\"setting_slider\">"
             "Mix a color:<br>"

             "<table>"
             "<tr><td>Red:</td><td><input type=\"range\" name=\"red_sl\" min=\"0\" max=\"255\" value=\"";
  message += r;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "<tr><td>Green:</td><td><input type=\"range\" name=\"green_sl\" min=\"0\" max=\"255\" value=\"";
  message += g;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "<tr><td>Blue:</td><td><input type=\"range\" name=\"blue_sl\" min=\"0\" max=\"255\" value=\"";
  message += b;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "</table>";
  
  message += "<br><input type=\"submit\" value=\"Submit\"></form>";
  message += "<script>function mysubmit() {document.getElementById(\"pickForm\").submit();}</script>";
  message += "<script>function slsubmit() {document.getElementById(\"slideForm\").submit();}</script>";
  message += "</body></html>";

  

  message += "try: <a href=\"https://carlos-aguayo.github.io/tfjs.html\">aiworker</a><br>";

  server.send(200, "text/html", message);
}


void handleSetting()
{

  long int tmp_int;

  String message = "<!DOCTYPE html><html><head>";
  message += "<meta http-equiv=\"refresh\" content=\"0; URL=/\">";
  message += "<title>AI socket</title></head><body>";
  message += "Setting AI socket RGB.<br>";

  if (server.args() != 1)
  {
    message += "Wrong arguments. Start from root.";
  }
  else
  {
    server.arg(0).substring(1, 7).toCharArray(rgbstring, 7);

    tmp_int = strtol(rgbstring, NULL, 16);
    //    message += tmp_int;
    //    message += "<br>";

    r = tmp_int >> 16;
    g = (tmp_int >> 8) & 0xFF;
    b = tmp_int & 0xFF;

    ChangePalettePeriodically();
  };

  message += "</body></html>";
  server.send(200, "text/html", message);
}


void handleSettingSlider()
{
  char tmp_string[4] = "128";

  String message = "<!DOCTYPE html><html><head>";
  message += "<meta http-equiv=\"refresh\" content=\"0; URL=/\">";
  message += "<title>AI socket</title></head><body>";
  message += "Setting AI socket RGB<br>";

  if (server.args() != 3)
  {
    message += "Wrong arguments. Start from root.";
  }
  else
  {
    server.arg(0).toCharArray(tmp_string, 4);
    r = strtol(tmp_string, NULL, 10);

    server.arg(1).toCharArray(tmp_string, 4);
    g = strtol(tmp_string, NULL, 10);

    server.arg(2).toCharArray(tmp_string, 4);
    b = strtol(tmp_string, NULL, 10);

    sprintf(rgbstring, "%02x%02x%02x", r, g, b);

    ChangePalettePeriodically();
  };

  message += "</body></html>";
  server.send(200, "text/html", message);
}
