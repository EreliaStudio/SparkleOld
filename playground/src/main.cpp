#include "playground.hpp"
#include <assert.h>

int main() {
    // Testing Default constructor
    spk::Value<int>::Default defaultValue(10);
    assert(static_cast<int>(defaultValue) == 10);

    // Testing Default copy constructor
    spk::Value<int>::Default copiedDefaultValue(defaultValue);
    assert(static_cast<int>(copiedDefaultValue) == 10);

    // Testing Value constructor with Default
    spk::Value<int> valueFromDefault(defaultValue);
    assert(static_cast<int>(valueFromDefault) == 10);

    // Testing Value copy constructor
    spk::Value<int> copiedValue(valueFromDefault);
    assert(static_cast<int>(copiedValue) == 10);

    // Testing Value constructor with Default and custom value
    spk::Value<int> customValue(defaultValue, 20);
    assert(static_cast<int>(customValue) == 20);

    // Testing Value assignment operator with Default
    defaultValue = 15;
    valueFromDefault = defaultValue;
    assert(static_cast<int>(valueFromDefault) == 15);

    // Testing Value assignment operator with Value
    copiedValue = customValue;
    assert(static_cast<int>(copiedValue) == 20);

    // Testing Value assignment operator with TType
    valueFromDefault = 30;
    assert(static_cast<int>(valueFromDefault) == 30);

    // Testing Value reset function
    valueFromDefault.reset();
    assert(static_cast<int>(valueFromDefault) == 15);

	// Assign a custom value
    valueFromDefault = 100;

    // Test callback subscription and triggering
    bool callbackTriggered = false;
    auto callback = [&callbackTriggered]() { callbackTriggered = true; };

    // Subscribe to callback
    auto contract = valueFromDefault.subscribe(callback);


    // Change default value while on custom should not trigger callback
    defaultValue = 80;
    assert(callbackTriggered == false);

    // Change value to trigger callback
    valueFromDefault = 50;
    assert(callbackTriggered == true);

    return 0;
}