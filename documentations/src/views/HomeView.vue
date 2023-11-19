<script setup lang="ts">
import AstroCard from "@/components/AstroCard.vue";
import Footer from "@/components/Footer.vue";
import TerminalContent from "@/components/TerminalContent.vue";

declare class Termynal {
    constructor(id: string);
    constructor(id: string, options: any);
}

setTimeout(()=> {
    let done = false;
    new Termynal("#termynal", {
        ondone: ()=> done = true
    });

    let term : HTMLElement | null = document.getElementById("termynal");
    setInterval(function() {
        if(term != null && !done)
            term.scrollTop = term.scrollHeight;
    }, 100);
}, 10);
</script>

<template>
    <div class="row equal-cols">
        <div class="col-lg-6">
            <AstroCard>
                <div align="center">
                    <img style="width: 280px" src="@/assets/jessy_os_logo.png" />
                </div>
                <br/>

                <h1>What is Jessy OS?</h1>
                <hr/>
                <p>Jessy OS is an embedded operating system designed for the ESP32 microcontroller. It provides a versatile platform for various IoT and embedded systems projects, with features like a file system, real-time clock support, networking capabilities, user accounts, and JavaScript execution via the embedded Duktape VM.</p>
            </AstroCard>
        </div>

        <div class="col-lg-6">
            <AstroCard class="desktop-only">
                <div class="frame-btn"></div>

                <div id="termynal" data-termynal data-ty-typeDelay="90" data-ty-lineDelay="300">
                    <TerminalContent />
                </div>
            </AstroCard>
        </div>
    </div>
    <br class="desktop-only" />

    <AstroCard>
        <center class="pt-4">
            <h1>Features</h1>
            <p>Jessy OS provides a number of features that make it well-suited for IoT and embedded systems projects.</p>
            <hr/>
        </center>
        <br/>

        <div class="row">
            <div class="col-lg-4">
                <div align="center">
                    <h1><i class="bi bi-folder2-open"></i></h1><br/>
                    <h2>File System</h2>
                    <p>Jessy OS includes its own file system, allowing you to manage and access files on the ESP32 device.</p>
                </div>
                <br/>
            </div>

            <div class="col-lg-4">
                <div align="center">
                    <h1><i class="bi bi-watch"></i></h1><br/>
                    <h2>Real-time Clock</h2>
                    <p>It supports the DS1307 real-time clock via I2C, enabling accurate timekeeping for your applications.</p>
                </div>
                <br/>
            </div>

            <div class="col-lg-4">
                <div align="center">
                    <h1><i class="bi bi-people-fill"></i></h1><br/>
                    <h2>User Accounts</h2>
                    <p>You can create and manage multiple user accounts, making it suitable for applications requiring user authentication.</p>
                </div>
                <br/>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-2"></div>

            <div class="col-lg-4">
                <div align="center">
                    <h1><i class="bi bi-wifi"></i></h1><br/>
                    <h2>IoT / Networking</h2>
                    <p>Jessy OS includes networking functionality with a WLAN command for managing Wi-Fi connections and network configurations.</p>
                </div>
                <br/>
            </div>

            <div class="col-lg-4">
                <div align="center">
                    <h1><i class="bi bi-filetype-js"></i></h1><br/>
                    <h2>JavaScript Support</h2>
                    <p>The embedded Duktape VM allows you to execute JavaScript programs, expanding the range of applications you can build.</p>
                </div>
                <br/>
            </div>

            <div class="col-lg-2"></div>
        </div>
    </AstroCard>
    <br/>

    <Footer />
</template>
