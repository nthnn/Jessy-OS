import {createRouter, createWebHistory} from "vue-router";

import DocumentationsView from "../views/DocumentationsView.vue";
import DonateView from "../views/DonateView.vue";
import HomeView from "../views/HomeView.vue";

const router = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    routes: [
        {
            path: "/",
            name: "home",
            component: HomeView
        },
        {
            path: "/documentations",
            name: "documentations",
            component: DocumentationsView
        },
        {
            path: "/donate",
            name: "donate",
            component: DonateView
        }
    ]
});

export default router;
