// E-paper display
#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.cpp>      // 2.13" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Other libs
#include <RBD_Button.h>
#include "Paper.h"


GxIO_Class io(SPI, SS, 0, 2);
GxEPD_Class display(io);
Lifeboxes::Paper paper(&display);
RBD::Button downButton(5);
RBD::Button upButton(12);

uint8_t currentPage = 1;


const char* recipe = R"END(
World's Best Bread (tm)

---

# INGREDIENTS

625 g wheat flour
500 ml water
2-3 tsp salt
1/4 tsp dry yeast

# METHOD

1. Mix yeast and salt into 5dl of warm water. This is really how little yeast you need!

2. Mix the flour into the water.

3. Keep mixing until there are no dry flour bits left (add more water if necessary).

4. Cover and let it rise in a warm place for 12 hours. Yes, 12 hours!

5. Pour out the dough and gently fold it onto itself in thirds 3-4 times.

6. Prepare a bowl with some olive oil and flour to prevent the dough from sticking.

7. Put the dough in the bowl, cover with plastic wrap, and let it rise for another 2 hours.

8. Put an oven-resistant pot with its lid on into your oven and preheat it to 250C. You want to make sure the pot is VERY HOT or the bread will stick when you bake it.

9. Once the oven is ready, take out the pot, pour the dough in (you should hear a sizzling sound if it's hot enough), put the lid back on and bake for 30 mins at 250C

10. Take the lid off, reduce temperature to 230C, bake for another 15 minutes and you're done!
)END";

const char* recipe2 = R"END(
World's Best Bread (tm)

---

- Mix 500ml water, yeast (1/4 tsp dry), and 2tsp salt with 625g flour.

- Cover and rise for 12 hours. Fold in thirds, rise 2 hours in bowl with oil/flour.

- Preheat pot at 250C, bake 30mins with lid, bake 15mins at 230 without lid.
)END";


void setup() {
  Serial.begin(115200);
  paper.renderText(recipe2);
}

void loop() {

  if(downButton.onPressed()) {
    Serial.println("Down Pressed");
    currentPage++;
    paper.renderText(recipe2, currentPage, false);
  }

  if(upButton.onPressed()) {
    Serial.println("Up Pressed");
    currentPage--;
    paper.renderText(recipe2, currentPage, false);
  }
  
}

void loadRecipe() {
}

