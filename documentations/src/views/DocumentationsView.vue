<script setup lang="ts">
import AstroCard from "@/components/AstroCard.vue";
import Documentation from "@/components/Documentation.vue";
import TableOfContents from "@/components/TableOfContents.vue";
</script>

<template>
    <br/>

    <div class="row">
        <div class="col-lg-5">
            <AstroCard>
                <TableOfContents />
            </AstroCard>
            <br />
        </div>

        <div class="col-lg-7">
            <AstroCard>
                <Documentation />
            </AstroCard>
        </div>
    </div>
</template>