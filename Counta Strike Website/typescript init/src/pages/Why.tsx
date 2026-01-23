import type { ReactNode, CSSProperties } from "react";
import RenderTextOffsets from "../components/CoolOffsetText/RenderTextOffsets.tsx";


const Why = () => {
    return (
        <div style={styles.root}>
            {/* title */}
            <div style={styles.title}>
                <div>Counter-Strike 2</div>
                <div>What is this?</div>
            </div>

            {/* body */}
            <div style={styles.card}>
				<div>
					This project reveals Counter-Strike 2 offsets, memory patterns, and more!
				</div>

				{/* do a starwars intro theme kinda */}
				<RenderTextOffsets />


            </div>
        </div>
    );
}

const styles: Record<"root" | "title" | "card", CSSProperties> = {
    root: {
        fontSize: "1.5rem",
        padding: 12,
		color: "lightsteelblue",

    },
    title: {
        fontSize: "2rem",
        margin: 0,
        color: "gold",
		textShadow: "1px 2px #B59410",
		textAlign: "center"
    },
	card: {
		maxWidth: 800,
		margin: "16px auto 0",
		padding: 12,
		borderRadius: 8,
		display: "flex",
		flexDirection: "column",
		justifyContent: "center",
	}
};
export default Why;
