<script setup lang="ts">
import TabLink from "@/components/TabLink.vue";
</script>

<template>
    <p>This steps should be followed if intended just to be a prototype or experimental.</p>
    <br/>

    <h4>Components</h4>
    <p>If you don't want to build your own Jessy OS single-board PCB or Jessy OS dedicated shields, you can follow the connections for each components below.</p>
    <p>Before getting started with Jessy OS, you'll need the following:</p>
    <ul>
        <li>An ESP32 microcontroller.</li>
        <li>A compatible SD card adapter module for file system support.</li>
        <li>A DS1307 real-time clock module connected via I2C.</li>
    </ul>

    <p>After gathering the aforementioned components, connect it as follows:</p>
    <center>
        <div class="border-gradient w-75">
            <table class="table table-hover table-dark border-gradient m-0">
                <thead align="center">
                    <tr>
                        <td class="p-2">Component</td>
                        <td class="p-2">ESP32</td>
                    </tr>
                </thead>
                <tbody align="center">
                    <tr><td colspan="2" class="p-2" align="center">SD Card Adapter</td></tr>
                    <tr>
                        <td class="p-2">VCC</td>
                        <td class="p-2">5V</td>
                    </tr>
                    <tr>
                        <td class="p-2">GND</td>
                        <td class="p-2">GND</td>
                    </tr>
                    <tr>
                        <td class="p-2">CS</td>
                        <td class="p-2">GPIO5</td>
                    </tr>
                    <tr>
                        <td class="p-2">MOSI</td>
                        <td class="p-2">GPIO23</td>
                    </tr>
                    <tr>
                        <td class="p-2">MISO</td>
                        <td class="p-2">GPIO19</td>
                    </tr>
                    <tr>
                        <td class="p-2">SCK</td>
                        <td class="p-2">GPIO18</td>
                    </tr>
                    <tr><td colspan="2" class="p-2" align="center">DS1307 Real-time Clock</td></tr>
                    <tr>
                        <td class="p-2">VCC</td>
                        <td class="p-2">VCC</td>
                    </tr>
                    <tr>
                        <td class="p-2">GND</td>
                        <td class="p-2">GND</td>
                    </tr>
                    <tr>
                        <td class="p-2">SCL</td>
                        <td class="p-2">SCL</td>
                    </tr>
                    <tr>
                        <td class="p-2">SDA</td>
                        <td class="p-2">SDA</td>
                    </tr>
                </tbody>
            </table>
        </div>
    </center>
    <br/>

    <h4>Flashing the OS</h4>
    <ol>
        <li>
            <p>Clone the Jessy OS repository.</p>
            <pre class="border-gradient bg-dark p-2">git clone https://github.com/nthnn/Jessy-OS.git</pre>
        </li>
        <li><p>Open the clone repository via PlatformIO extension on VS Code.</p></li>
        <li><p>Plug your board in, and then press the upload button on the status bar of VS Code.</p></li>
        <li><p>Enjoy!</p></li>
    </ol>
    <br/>

    <h4>Running</h4>
    <p>Communication to Jessy OS can be done through UART serial communication, which means, it just needs to be connected to the USB port (e.g. via CH340, PL2302, etc).</p>
    <ol>
        <li><p>Connect the ESP32 (e.g. ESP32 DevKit 1.0 or the JessyOS PCB) board to USB port.</p></li>
        <li><p>Open the <span class="border-gradient px-2 py-1 bg-dark">jsym</span>. Program dedicated for communicating with Jessy OS via UART.</p></li>
        <li><p>Press the reset button on ESP32 DevKit board.</p></li>
    </ol>
</template>