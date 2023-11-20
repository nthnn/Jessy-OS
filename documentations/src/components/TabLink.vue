<script setup lang="ts">
const props = defineProps<{
    title: string,
    id: string,
    isActive: boolean
}>();

let navItemClass = "nav-link text-white" + (props.isActive ? " active" : ""),
    pillId = "v-pills-" + props.id,
    hpillId = "#" + pillId,
    label = pillId + "-tab";
</script>

<template>
    <button :class="navItemClass" :id="label"
        data-bs-toggle="pill" :data-bs-target="hpillId"
        type="button" role="tab" :aria-controls="pillId"
        aria-selected="true">&mdash; {{ title }}</button>
</template>