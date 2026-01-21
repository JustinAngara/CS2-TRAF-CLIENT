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
				This project reveals Counter-Strike 2 offsets, memory patterns, and more!
            </div>
        </div>
    );
}

const styles: Record<"root" | "title", CSSProperties> = {
    root: {
        fontSize: "1.5rem",
        padding: 12,
		color: "lightsteelblue"

    },
    title: {
        fontSize: "2rem",
        margin: 0,
        color: "gold",
		textShadow: "1px 2px #B59410"
    }
};
export default Why;
