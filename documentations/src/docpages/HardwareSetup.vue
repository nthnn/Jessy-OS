<script setup lang="ts">
import TabLink from "@/components/TabLink.vue";
import type { RouterLink } from 'vue-router';
</script>

<template>
    <h4>Do-It-Yourself Setup</h4>
    <ol>
        <li><p>Download the Gerber file from folder <a href="https://github.com/nthnn/Jessy-OS/blob/main/pcb_files" target="_blank" class="text-decoration-none">pcb_files</a>.</p></li>
        <li><p>Go to any of your favorite PCB manufacturer to fabricate the Jessy-OS PCB or alternatively, you can go to our <RouterLink to="/store">store</RouterLink>.</p></li>
        <li><p>After soldering all the necessary components, you can now proceed to flashing the OS to Jessy OS which was explained from the previous page.</p></li>
        <li><p>Copy the folder <a href="https://github.com/nthnn/Jessy-OS/tree/main/sd" target="_blank" class="text-decoration-none">sd</a> to your SD card that will be using on Jessy OS. Do not forget to delete the <span class="border-gradient px-2 py-1">removethisfile</span> files inside each folder.</p></li>
    </ol>
    <br/>

    <h4>PCB Variations</h4>
    <p>There are different variations of available open source PCBs for Jessy OS. Below are the following:</p>
    <ul>
        <li><p><a href="#" class="text-decoration-none">Jessy OS ModuloBoard</a> &mdash; This board is a single PCB that requires three key components other than header pins; which are NodeMCU ESP32, DeekRobot's data logger module, and a pigtail IPEX antenna.</p></li>
        <li><p><a href="#" class="text-decoration-none">Jessy OS Shield</a> &mdash; NodeMCU shield for ESP32 that can be easily plugged in and out, which contains the required real-time clock and SD card slot.</p></li>
        <li><p><a href="#" class="text-decoration-none">Jessy OS Power Supply Shield</a> &mdash; Optional stackable shield for Jessy OS Shield for power supply that uses two CR2032 with a toggle switch.</p></li>
        <li><p><a href="#" class="text-decoration-none">Jessy OS Prototyping Shield</a> &mdash; Stackable shield for prototyping which intentionally used altogether with the other shields.</p></li>
    </ul>
</template>