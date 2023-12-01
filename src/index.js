import React from 'react';
import ReactDOM from 'react-dom/client';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import './globals.css';
import App from './App';
import NotFound from './404';
import GranulePage from './Granule';

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <React.StrictMode>
    <Router>
      <Routes>
        <Route path='/' element=<App /> />
        <Route path='/granule/:id/:page' element=<GranulePage /> />
        <Route path='*' element=<NotFound /> />
      </Routes>
    </Router>
  </React.StrictMode>
);
