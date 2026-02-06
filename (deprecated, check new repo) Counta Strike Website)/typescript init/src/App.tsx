// import './App.css'
import { Outlet } from "react-router-dom";
import NavbarComponent from "./components/NavbarComponent";

const App = () => {

    return (
        <div style={styles.app}>
            {/* entry point just load all components that make up this */}
            <NavbarComponent />

            {/* main content area */}
            <main>
                <Outlet />

            </main>
        </div>
    );
}

const styles: Record<string, React.CSSProperties> = {
    app: {
        minHeight: "100vh",
        backgroundColor: "rgb(25,25,25)",
        color: "white"
    },

};

export default App
