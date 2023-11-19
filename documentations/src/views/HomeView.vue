<script setup lang="ts">
import AstroCard from "@/components/AstroCard.vue";
import Footer from "@/components/Footer.vue";

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
            <AstroCard>
                <div class="frame-btn"></div>

                <div id="termynal" data-termynal data-ty-typeDelay="100" data-ty-lineDelay="800">
                    <span data-ty="input" data-ty-prompt=" anonymous@root "> echo "Hello, world!"</span>
                    <span data-ty>Hello, world!</span>

                    <span data-ty="input" data-ty-prompt=" anonymous@root "> mkdir new_folder</span>
                    <span data-ty="input" data-ty-prompt=" anonymous@root "> touch test.txt</span>

                    <span data-ty="input" data-ty-prompt=" anonymous@root "> ls</span>
                    <span data-ty>new_folder<br/>test.txt</span>
                </div>
            </AstroCard>
        </div>
    </div>
    <br/>

    <Footer />
</template>
