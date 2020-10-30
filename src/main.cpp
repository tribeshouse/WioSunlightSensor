#include <Arduino.h>
#include"seeed_line_chart.h" //include the library
TFT_eSPI tft;
#include "SI114X.h"
SI114X SI1145 = SI114X();

#define max_size 50 //maximum size of data
#define num_charts 3 //number of charts
#define label_width 16

doubles data[num_charts]; //Initilising a doubles type to store data
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite

void setup() {
    tft.begin();
    tft.setRotation(3);
    spr.createSprite(TFT_HEIGHT,TFT_WIDTH);
    while (!SI1145.Begin()) {
        delay(1000);
    }
  }

void loop() {
    spr.fillSprite(TFT_WHITE);

    float vis = SI1145.ReadVisible();
    float ir = SI1145.ReadIR();
    float uv = SI1145.ReadUV();

    if (data[0].size() == max_size) {
      for (uint8_t i = 0; i<num_charts; i++){
        data[i].pop(); //this is used to remove the first read variable
      }
    }
    data[0].push(vis);
    data[1].push(ir);
    data[2].push(uv);

    //Settings for the line graph title
    auto header =  text(0, 0)
                .value("vis/ir/uv")
                .align(center)
                .valign(vcenter)
                .width(tft.width())
                .thickness(3);

    header.height(header.font_height() * 2);
    header.draw(); //Header height is twice the height of the font

    //Write data to screen
    int label_height = (tft.height() - header.height()*1.5) / num_charts;
    char str[6];

    dtostrf(vis,5,1,str);
    auto label1 = text(tft.width()-label_width-5,header.height() + label_height * 0);
         label1
                .value(str)
                .width(label_width)
                .height(label_height)
                .align(right)
                .valign(vcenter)
                .color(TFT_ORANGE)
                .draw()
                ;

    dtostrf(ir,5,1,str);
    auto label2 = text(tft.width()-label_width-5,header.height() + label_height * 1);
         label2
                .value(str)
                .width(label_width)
                .height(label_height)
                .align(right)
                .valign(vcenter)
                .color(TFT_MAROON)
                .draw()
                ;

    dtostrf(uv,5,1,str);
    auto label3 = text(tft.width()-label_width-5,header.height() + label_height * 2);
         label3
                .value(str)
                .width(label_width)
                .height(label_height)
                .align(right)
                .valign(vcenter)
                .color(TFT_PURPLE)
                .draw()
                ;

  //Settings for the line graph
    auto content1 = line_chart(20, header.height()); //(x,y) where the line graph begins
         content1
                .height((tft.height() - header.height() * 1.5)) //actual height of the line chart
                .width(tft.width() - content1.x() * 2 - label_width) //actual width of the line chart
                .based_on(0.0) //Starting point of y-axis, must be a float
                .show_circle(false) //drawing a cirle at each point, default is on.
                .value({data[0],data[1],data[2]}) //passing through the data to line graph
                .color(TFT_ORANGE,TFT_MAROON,TFT_PURPLE) //Setting the color for the line
                .draw();

    spr.pushSprite(0, 0);
    delay(1000);
}
