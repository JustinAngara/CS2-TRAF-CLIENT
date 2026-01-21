import type { ReactNode, CSSProperties } from "react";


const Why = () => {
    return (
        <div style={styles.root}>
            {/* title */}
            <div style={styles.title}>
                <div>Counter-Strike 2</div>
                <div>What is this?</div>
            </div>

            {/* body */}
            <div>
                Basically i wanted to create a lot of hacks
            </div>
        </div>
    );
}

const styles: Record<"root" | "title", CSSProperties> = {
    root: {
        fontSize: "1.5rem",
        padding: 12,

    },
    title: {
        fontSize: "2rem",
        margin: 0,
        color: "gold"
    }
};
export default Why;
