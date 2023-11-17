<script setup lang="ts">
const props = defineProps<{
    text: string,
    route: string,
    isActive: boolean
}>()

let navItemClass = "nav-link" + (props.isActive ? " active" : "");
</script>

<template>
    <li class="nav-item">
        <a :class="navItemClass">
            <RouterLink :to="route">{{ text }}</RouterLink>
        </a>
    </li>
</template>