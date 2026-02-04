import type { CSSProperties } from "react";
import OffsetCard, { type OffsetBlock } from "./OffsetCard";

type OffsetsJson =
	| {
	data?: OffsetBlock[] | null;
}
	| null;

type DisplayOffsetsProps = {
	data: OffsetsJson;
	title?: string;
};

const DisplayOffsets = ({ data, title = "Offsets" }: DisplayOffsetsProps) => {
	const blocks: OffsetBlock[] = Array.isArray(data?.data) ? data!.data! : [];

	return (
		<div style={styles.root}>
			<h2 style={styles.title}>{title}</h2>

			{blocks.length === 0 ? (
				<div style={styles.empty}>No data</div>
			) : (
				<div style={styles.masonry}>
					{blocks.map((block, i) => (
						<OffsetCard key={(block && (block as any).Key) ?? i} block={block} />
					))}
				</div>
			)}
		</div>
	);
};

const styles: Record<"root" | "title" | "masonry" | "empty", CSSProperties> = {
	root: { padding: 12 },
	title: { margin: "0 0 12px 0" },
	empty: { opacity: 0.75, fontSize: 13 },


	masonry: {
		display: "block",        // overrides any inherited grid/flex
		columnCount: 5,
		columnGap: 12,
		width: "100%",
	},
};

export default DisplayOffsets;
