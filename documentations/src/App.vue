<script setup lang="ts">
import { watch } from "vue";
import { useRoute, RouterView } from "vue-router";
import NavbarItem from "./components/NavbarItem.vue";

import "@/assets/bootstrap.min.css";

const route = useRoute();

watch(
    () => route.fullPath,
    async (n, nn) => window.scrollTo(0, 0)
);
</script>

<template>
    <nav class="navbar navbar-expand-lg bg-primary fixed-top" data-bs-theme="dark">
        <div class="container-fluid">
            <a class="navbar-brand" href="#">Jessy OS</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarColor01" aria-controls="navbarColor01" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
    
            <div class="collapse navbar-collapse justify-content-end" id="navbarColor01">
                <ul class="navbar-nav">
                    <NavbarItem route="/" text="Home" is-active></NavbarItem>
                    <NavbarItem route="/documentations" text="Documentations" :is-active="false"></NavbarItem>
                    <NavbarItem route="/api-refs" text="API Reference" :is-active="false"></NavbarItem>
                    <li class="nav-item">
                        <a href="https://github.com/nthnn/Jessy-OS" class="nav-link" target="_blank">GitHub</a>
                    </li>
                    <NavbarItem route="/donate" text="Donate" :is-active="false"></NavbarItem>
                </ul>
            </div>
        </div>
    </nav>
    <br/><br/><br/>

    <div class="container p-4">
        <RouterView />
    </div>
</template>