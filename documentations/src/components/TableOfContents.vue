<script setup lang="ts">
import TabLink from "./TabLink.vue";
</script>

<template>
    <h1>Documentations</h1>
    <hr/>

    <h3>Overview</h3>
    <div class="nav nav-pills me-3 d-block" id="v-pills-tab" role="tablist" aria-orientation="vertical">
        <TabLink title="Getting Started" id="getting-started" is-active />
        <TabLink title="Hardware Setup" id="hardware-setup" :is-active="false" />
        <TabLink title="Contributing" id="contributing" :is-active="false" />
        <TabLink title="License" id="license" :is-active="false" />
    </div>
    <hr/>

    <h3>Terminal Commands</h3>
</template>