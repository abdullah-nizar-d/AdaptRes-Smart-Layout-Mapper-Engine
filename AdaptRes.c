#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Cache for width and height percentages (1% to 100%)
int width_percent[101];  // Precomputed 1% to 100% of the display width
int height_percent[101]; // Precomputed 1% to 100% of the display height
bool is_external_display_connected = false; // Flag for external display connection

// Function to precompute width and height percentages for 1% to 100%
void precompute_percentages(int width, int height) {
    for (int i = 1; i <= 100; i++) {
        width_percent[i] = (width * i) / 100;
        height_percent[i] = (height * i) / 100;
    }
    printf("Precomputed percentages for width=%d, height=%d\n", width, height);
}

// Function to clamp a percentage to the range [1, 100]
int clamp_percentage(int percent) {
    if (percent < 1) return 1;
    if (percent > 100) return 100;
    return percent;
}

// Function to convert UI elements' position and size using precomputed percentage cache
void update_ui_from_percentage(int x_percent, int y_percent, int width_percent_value, int height_percent_value) {
    if (!is_external_display_connected) {
        printf("No external display connected. Cannot update UI.\n");
        return;
    }

    // Clamp the percentage values to be within [1, 100]
    x_percent = clamp_percentage(x_percent);
    y_percent = clamp_percentage(y_percent);
    width_percent_value = clamp_percentage(width_percent_value);
    height_percent_value = clamp_percentage(height_percent_value);

    // Lookup precomputed percentage positions and dimensions from cache
    int x_position = width_percent[x_percent];   // Get x-position from the width cache
    int y_position = height_percent[y_percent];  // Get y-position from the height cache
    int element_width = width_percent[width_percent_value];  // Get width from the cache
    int element_height = height_percent[height_percent_value]; // Get height from the cache

    printf("UI Element Position and Size: x=%d, y=%d, width=%d, height=%d (Input: x=%d%%, y=%d%%, width=%d%%, height=%d%%)\n", 
           x_position, y_position, element_width, element_height, 
           x_percent, y_percent, width_percent_value, height_percent_value);
}

// Function to connect an external display, compute the cache, and set display state
void connect_external_display(int width, int height) {
    is_external_display_connected = true;
    precompute_percentages(width, height); // Precompute percentages (1% to 100%) for the given resolution
    printf("External display connected with resolution %dx%d\n", width, height);
}

// Function to disconnect the external display and clear the cache
void disconnect_external_display() {
    is_external_display_connected = false;
    for (int i = 1; i <= 100; i++) {
        width_percent[i] = 0;
        height_percent[i] = 0;
    }
    printf("External display disconnected and cache cleared.\n");
}

// Main function to simulate the connection, layout computation, and disconnection
int main() {
    // 1. Connect external display (for example, 1920x1080)
    connect_external_display(1920, 1080);

    // 2. Simulate layout positions for UI elements
    update_ui_from_percentage(10, 5, 20, 10);  // 10% x, 5% y, 20% width, 10% height
    update_ui_from_percentage(150, 200, 300, -50); // Clamp values (150 -> 100, 200 -> 100, 300 -> 100, -50 -> 1)
    update_ui_from_percentage(50, 50, 40, 40);  // 50% x, 50% y, 40% width, 40% height
    update_ui_from_percentage(-10, 0, 101, 999); // Clamp values (-10 -> 1, 0 -> 1, 101 -> 100, 999 -> 100)

    // 3. Disconnect external display
    disconnect_external_display();

    // 4. Attempt to update layout after disconnection (should fail)
    update_ui_from_percentage(10, 5, 20, 10);

    return 0;
}