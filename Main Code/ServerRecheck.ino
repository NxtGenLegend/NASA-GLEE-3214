#include <GLEE_Radio.h>

// Initialize RF Object
LunaRadio Rad;

// Flag to indicate that a packet was received
volatile bool messageRecieved = false;

// Disable interrupt when it's not needed
volatile bool interruptEnabled = true;

String rqst;

int turn = 1;

void recieve_callback(void)
{
    // Don't set flag if interrupt isn't enabled
    if (!interruptEnabled)
    {
        return;
    }

    // Set flag signifying message was recieved
    messageRecieved = true;
}

void setup()
{
    // Set the data rate to 9600 bits per second
    Serial.begin(9600);

    // Initialize the radio settings by using the initialize_radio function
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0, 17, 250.0, 12, 8);

    Rad.enable_recieve_interupt(recieve_callback);
}

void loop()
{
    if (messageRecieved)
    {
        // Disable interrupts during reception processing

        interruptEnabled = false;

        // Reset reception flag
        messageRecieved = false;

        byte data_buffer[8];

        Rad.readData(data_buffer, 8);
        rqst = String((char *)data_buffer);
        // Print data of the packet
        Serial.print(F("Data:\t\t"));
        Serial.println(rqst);

        // Return to listening for transmissions
        Rad.startRecieve();
        // We're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    // Send alternating messages every other second
    if (millis() % 1000 == 0)
    {
        if (turn == 1)
        {
            Rad.transmit_data("1");
            turn = 2;
        }
        else if (turn == 2)
        {
            Rad.transmit_data("2");
            turn = 1;
        }
    }
}