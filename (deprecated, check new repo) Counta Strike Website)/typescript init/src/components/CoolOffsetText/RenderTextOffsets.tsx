import type { CSSProperties } from "react";
import OffsetCard, { type OffsetBlock } from "./OffsetCard";

type OffsetsJson = {
	data: OffsetBlock[] | null;
};

type DisplayOffsetsProps = {
	data: OffsetsJson;   // pass the full imported json object
	title?: string;
};

const DisplayOffsets = ({ data, title = "Offsets" }: DisplayOffsetsProps) => {
	const blocks = data?.data ?? [];

	return (
		<div style={styles.root}>
			<h2 style={styles.title}>{title}</h2>

			<div style={styles.grid}>
				{blocks.map((block, i) => (
					<OffsetCard key={block?.Key ?? i} block={block} />
				))}
			</div>
		</div>
	);
};

const styles: Record<"root" | "title" | "grid", CSSProperties> = {
	root: { padding: 12 },
	title: { margin: "0 0 12px 0" },

	grid: {
		display: "grid",
		gridTemplateColumns: "repeat(auto-fit, minmax(360px, 1fr))",
		gap: 12,
	},
};

export default DisplayOffsets;
