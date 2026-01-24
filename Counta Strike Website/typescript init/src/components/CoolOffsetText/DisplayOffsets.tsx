import type { ReactNode, CSSProperties } from "react";
import type { Data, Content} from "../../types/player.ts"

type DisplayOffsetsProps = {
	dll: string;
};
const DisplayOffsets = ({ dll }:DisplayOffsetsProps) => {
	const c:Content = {
		varType: "damn i am ass",
		varName: "lmao what the fuck",
		offset: "0x330"
	};

	const x = `this is offset ${c.offset}`;

	const data:Data = {
		content: c,
		size: 10
	};
    return (
        <div style={styles.root}>
			{ x }
        </div>
    );
}

const styles: Record<"root", CSSProperties> = {
    root: { padding: 12 },
};
export default DisplayOffsets;
