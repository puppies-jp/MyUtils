import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import HelloPage from "./user/page.tsx";
import {AddListPage} from "./user/page.tsx";
import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
    <BrowserRouter>
        <nav className="navbar navbar-expand-lg navbar-dark bg-dark">
            <Link className="navbar-brand" to="/">Home</Link>
            <Link className="navbar-brand" to="/hello">Hello</Link>
            <Link className="navbar-brand" to="/Add">Add</Link>
        </nav>
        <Routes>
            <Route path="/" element={<App />} />
            <Route path="/hello" element={<HelloPage />} />
            <Route path="/Add" element={<AddListPage />} />
        </Routes>
    </BrowserRouter>
);
