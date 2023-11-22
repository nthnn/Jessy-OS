<script setup lang="ts">
const props = defineProps<{
    title: string,
    id: string,
    isActive: boolean
}>();

let navItemClass = "tab-pane fade show" + (props.isActive ? " active" : ""),
    pillId = "v-pills-" + props.id,
    label = pillId + "-tab";
</script>

<template>
    <div :class="navItemClass" :id="pillId"
        role="tabpanel" :aria-labelledby="label">
        <h2>{{ title }}</h2>
        <hr/>

        <slot></slot>
    </div>
</template>