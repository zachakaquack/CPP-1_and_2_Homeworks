#include <cmath>
#include <iostream>

struct Material {
    std::string material;
    float materialMass;
};

struct Widget {
    float surfaceArea;
    float volume;
    float lbMass;
    float kgMass;
    Material material;
};

float calcWidgetSurfaceAreaInches(float radius1, float height, float radius2) {

    // calculate larger dome
    float largerBase = M_PI * std::pow(radius1, 2);
    float largerDome = 2 * largerBase;

    // cylinder

    float cylLength = (2 * M_PI * radius2) * height;
    float cylBase = M_PI * std::pow(radius2, 2);

    // total surface area of the larger dome
    float largerTotal = largerDome + (largerBase - cylBase);

    // smaller sphere
    float smallerDome = 2 * M_PI * std::pow(radius2, 2);

    return largerTotal + cylLength + smallerDome;
}

// calculates the total volume of the widget in cubic inches
float calcWidgetVolumeInches(float radius1, float height, float radius2) {

    float larger = (2.0 / 3.0) * M_PI * std::pow(radius1, 3);
    float smaller = (2.0 / 3.0) * M_PI * std::pow(radius2, 3);
    float cylinder = M_PI * std::pow(radius2, 2) * height;

    return larger + smaller + cylinder;
}

// calculate the different masses
float calcWidgetMassPounds(float volume) { return volume * 0.0361295; }
float calcWidgetMassKilograms(float volume) { return volume * 0.0139636; }

// creates the actual widget and calculates it
Widget calcWidget(float radius1, float height, float radius2, Material mat) {

    Widget w;
    w.surfaceArea = calcWidgetSurfaceAreaInches(radius1, height, radius2);
    w.volume = calcWidgetVolumeInches(radius1, height, radius2);
    w.lbMass = calcWidgetMassPounds(w.volume * mat.materialMass);
    w.kgMass = calcWidgetMassKilograms(w.volume * mat.materialMass);
    w.material = mat;

    return w;
}

// literally just handles printing it
void printWidget(Widget w) {

    std::cout << "Widget's Surface Area: " << w.surfaceArea << " in^2" << '\n';
    std::cout << "Widget's Volume: " << w.volume << " in^3" << '\n';
    std::cout << "Widget's Mass: " << w.kgMass << " kg" << '\n';
    std::cout << "Widget's Mass: " << w.lbMass << " lb" << '\n';
    std::cout << "Widget's Material: " << w.material.material << " @ "
              << w.material.materialMass << " g/cm^3" << '\n';
    std::cout << "---" << '\n';
}

Material createMaterial(std::string material, float mass) {
    Material mat;
    mat.material = material;
    mat.materialMass = mass;
    return mat;
}

int main() {

    // introduction
    std::cout << "HW1 - Widget" << '\n';
    std::cout << "Solution by <zachakaquack>" << '\n' << '\n';

    // variables
    float radius1;
    float height;
    float radius2;

    // materials
    Material matAluminum = createMaterial("Aluminum", 2.7);
    Material matIron = createMaterial("Iron", 7.86);
    Material matTitanium = createMaterial("Titanium", 4.5);

    // inputting
    std::cout << "Enter the radius of the top (inches): ";
    std::cin >> radius1;

    std::cout << "Enter the length of the cylinder (inches): ";
    std::cin >> height;

    std::cout << "Enter the radius of the bottom (inches): ";
    std::cin >> radius2;
    std::cout << '\n';

    // creating the widgets
    Widget widgetAluminum = calcWidget(radius1, height, radius2, matAluminum);
    Widget widgetIron = calcWidget(radius1, height, radius2, matIron);
    Widget widgetTitanium = calcWidget(radius1, height, radius2, matTitanium);

    // printing the actual information (im not implementing reflection)
    printWidget(widgetAluminum);
    printWidget(widgetIron);
    printWidget(widgetTitanium);

    // closing
    std::cout << '\n' << "HW1 Complete" << '\n';

    return 0;
}
