import type { ReactNode, CSSProperties } from "react";
import type { Data, Content} from "../../types/player.ts"


const DisplayOffsets = () => {
	const c:Content = {
		varType: "damn i am ass",
		varName: "lmao what the fuck",
		offset: 0x04
	};

	const data:Data = {
		content: c,
		size: 10
	};
    return (
        <div style={styles.root}>

        </div>
    );
}

const styles: Record<"root", CSSProperties> = {
    root: { padding: 12 },
};
export default DisplayOffsets;
