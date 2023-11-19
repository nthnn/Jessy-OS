<script setup lang="ts">
import AstroCard from '@/components/AstroCard.vue';
</script>

<template>
    <br/>

    <div class="row">
        <div class="col-lg-5">
            <AstroCard>
                <h1>Documentations</h1>
                <hr/>

                <h3>Overview</h3>
                <div class="nav nav-pills me-3 d-block" id="v-pills-tab" role="tablist" aria-orientation="vertical">
                    <button class="nav-link active text-white" id="v-pills-getting-started-tab"
                        data-bs-toggle="pill" data-bs-target="#v-pills-getting-started"
                        type="button" role="tab" aria-controls="v-pills-getting-started"
                        aria-selected="true">&mdash; Getting Started</button>

                    <button class="nav-link text-white" id="v-pills-hardware-setup-tab"
                        data-bs-toggle="pill" data-bs-target="#v-pills-hardware-setup"
                        type="button" role="tab" aria-controls="v-pills-hardware-setup"
                        aria-selected="false">&mdash; Hardware Setup</button>

                    <button class="nav-link text-white" id="v-pills-contributing-tab"
                        data-bs-toggle="pill" data-bs-target="#v-pills-contributing"
                        type="button" role="tab" aria-controls="v-pills-contributing"
                        aria-selected="false">&mdash; Contributing</button>

                    <button class="nav-link text-white" id="v-pills-license-tab"
                        data-bs-toggle="pill" data-bs-target="#v-pills-license"
                        type="button" role="tab" aria-controls="v-pills-license"
                        aria-selected="false">&mdash; License</button>
                </div>
                <hr/>

                <h3>Terminal Commands</h3>
            </AstroCard>
            <br />
        </div>

        <div class="col-lg-7">
            <AstroCard>
                <div class="tab-content" id="v-pills-tabContent">
                    <div class="tab-pane fade show active" id="v-pills-getting-started"
                        role="tabpanel" aria-labelledby="v-pills-getting-started-tab">
                        <h3>Getting Started</h3>
                        <hr/>
                    </div>

                    <div class="tab-pane fade" id="v-pills-hardware-setup"
                        role="tabpanel" aria-labelledby="v-pills-hardware-setup-tab">
                        <h3>Hardware Setup</h3>
                        <hr/>
                    </div>

                    <div class="tab-pane fade" id="v-pills-contributing"
                        role="tabpanel" aria-labelledby="v-pills-contributing-tab">
                        <h3>Contributing Tab</h3>
                        <hr/>
                    </div>

                    <div class="tab-pane fade" id="v-pills-license"
                        role="tabpanel" aria-labelledby="v-pills-license-tab">
                        <h3>License</h3>
                        <hr/>
                    </div>
                </div>
            </AstroCard>
        </div>
    </div>
</template>