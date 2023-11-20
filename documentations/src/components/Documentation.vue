<script setup lang="ts">
import TabPage from "./TabPage.vue";
import GettingStarted from "../docpages/GettingStarted.vue";
</script>

<template>
    <div class="tab-content" id="v-pills-tabContent">
        <TabPage title="Getting Started" id="getting-started" is-active>
            <GettingStarted />
        </TabPage>

        <TabPage title="Hardware Setup" id="hardware-setup" :is-active="false">
        </TabPage>

        <TabPage title="Contributing" id="contributing" :is-active="false">
        </TabPage>

        <TabPage title="License" id="license" :is-active="false">
        </TabPage>
    </div>
</template>