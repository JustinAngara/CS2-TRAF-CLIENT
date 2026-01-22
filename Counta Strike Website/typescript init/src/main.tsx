import React from "react";
import ReactDOM from "react-dom/client";
import { RouterProvider, createBrowserRouter } from "react-router-dom";
import App from "./App";
import Why from "./pages/Why";
import Contact from "./pages/Contact";
import 'bootstrap/dist/css/bootstrap.min.css';

const router = createBrowserRouter([
    {
        path: "/",
        element: <App />, // entry point layout: navbar + outlet
        children: [
            { index: true, element: <Why /> },        // / (default)
            { path: "contact", element: <Contact /> } // /contact
        ],
    },
]);

ReactDOM.createRoot(document.getElementById("root")!).render(
    <React.StrictMode>
        <RouterProvider router={router} />
    </React.StrictMode>
);
