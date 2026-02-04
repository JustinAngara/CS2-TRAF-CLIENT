import type { CSSProperties } from "react";

export type OffsetItem = {
	Name?: string | null;
	Offset?: string | null;
	Type?: string | null;
};

export type OffsetBlock = {
	Items?: number | null;
	Key?: number | null;
	List?: OffsetItem[] | null;
	Name?: string | null;
	Type?: string | null;
} | null;

type OffsetCardProps = {
	block: OffsetBlock;
};

const OffsetCard = ({ block }: OffsetCardProps) => {
	if (!block) {
		return <div style={styles.card}>Invalid block</div>;
	}

	const key = block.Key ?? -1;
	const title = block.Name?.trim() ? block.Name : `Table ${key}`;
	const kind = block.Type?.trim() ? block.Type : "UNKNOWN";

	const list: OffsetItem[] = Array.isArray(block.List) ? block.List : [];
	const items = block.Items ?? list.length;

	return (
		<div style={styles.card}>
			<div style={styles.cardHeader}>
				<h3 style={styles.cardTitle}>{title}</h3>

				<div style={styles.metaRow}>
					<span style={kind === "NAMESPACE" ? styles.badgeNamespace : styles.badgeEnum}>
						{kind}
					</span>

					<span style={styles.muted}>Items: {items}</span>
				</div>
			</div>

			{list.length === 0 ? (
				<div style={styles.muted}>No items</div>
			) : (
				<div style={styles.list}>
					{/* header row */}
					<div style={{ ...styles.row, ...styles.headerRow }}>
						<div style={styles.hName}>Name</div>
						<div style={styles.hOffset}>Offset</div>
						<div style={styles.hType}>Type</div>
					</div>

					{list.map((it, idx) => (
						<div key={`${key}-${it.Name ?? "item"}-${idx}`} style={styles.row}>
							<div style={styles.name}>{it.Name ?? ""}</div>
							<div style={styles.offset}>{it.Offset ?? ""}</div>
							<div style={styles.type}>
								{(it.Type && it.Type.trim().length > 0) ? it.Type : "int32"}
							</div>
						</div>
					))}
				</div>
			)}
		</div>
	);
};

// @ts-ignore
const styles: Record<string, CSSProperties> = {

	card: {
		display: "inline-block",
		width: "100%",
		height: "auto",
		minHeight: 0,
		marginBottom: 40,
		breakInside: "avoid",
		border: "2px solid rgba(255,255,255,0.22)",
		boxShadow: "0 0 0 1px rgba(0,0,0,0.55) inset, 0 6px 18px rgba(0,0,0,0.35)",
		borderRadius: 8,
		overflow: "hidden",
	},


	cardHeader: {
		marginBottom: 10,
		paddingBottom: 10,
		borderBottom: "1px solid rgba(255,255,255,0.10)",
	},

	cardTitle: { margin: 0, fontSize: 16, fontWeight: 700 },

	metaRow: { display: "flex", gap: 10, alignItems: "center", marginTop: 8 },

	badge: {
		fontSize: 12,
		padding: "2px 8px",
		borderRadius: 999,
		border: "1px solid rgba(255,255,255,0.20)",
	},

	muted: { fontSize: 12, opacity: 0.75 },

	list: { display: "flex", flexDirection: "column", gap: 6 },

	headerRow: {
		background: "rgba(0,0,0,0.25)",
		border: "1px solid rgba(255,255,255,0.08)",
		fontSize: 12,
		opacity: 0.9,
	},

	row: {
		display: "grid",
		gridTemplateColumns: "1.7fr 0.7fr 1fr",
		gap: 10,
		alignItems: "start",

		padding: "6px 8px",
		borderRadius: 8,
		background: "rgba(0,0,0,0.15)",
	},

	// headers
	hName: { fontWeight: 700, opacity: 0.85 },
	hOffset: { fontWeight: 700, opacity: 0.85, fontFamily: "monospace" },
	hType: { fontWeight: 700, opacity: 0.85 },

	// CONTENT: remove ellipsis; allow wrapping
	name: {
		fontFamily: "monospace",
		fontSize: 13,
		whiteSpace: "normal",
		wordBreak: "break-word",
		overflowWrap: "anywhere",

		color: "#b26bff", // purple
		textShadow:
			"0 0 6px rgba(178,107,255,0.75), 0 0 14px rgba(178,107,255,0.35)",
	},
	offset: {
		fontFamily: "monospace",
		fontSize: 13,
		whiteSpace: "nowrap",
		opacity: 0.95,

		color: "#ff4d4d", // red
		textShadow:
			"0 0 6px rgba(255,77,77,0.75), 0 0 14px rgba(255,77,77,0.35)",
	},

	type: {
		fontSize: 12,
		opacity: 0.95,
		whiteSpace: "normal",
		wordBreak: "break-word",
		overflowWrap: "anywhere",

		color: "#ffd54a", // yellow
		textShadow: "0 0 6px rgba(255,213,74,0.75), 0 0 14px rgba(255,213,74,0.35)",
	},
	badgeNamespace: {
		fontSize: 12,
		padding: "2px 8px",
		borderRadius: 999,
		border: "1px solid rgba(90,170,255,0.55)",
		color: "#5aaaff",
		textShadow: "0 0 6px rgba(90,170,255,0.75), 0 0 14px rgba(90,170,255,0.35)",
	},

	badgeEnum: {
		fontSize: 12,
		padding: "2px 8px",
		borderRadius: 999,
		border: "1px solid rgba(255,255,255,0.55)",
		color: "#ffffff",

		background: "rgba(0,0,0,0.35)",

		textShadow:
			"0 0 2px rgba(255,255,255,0.95), " +
			"0 0 8px rgba(255,255,255,0.80), " +
			"0 0 18px rgba(255,255,255,0.55), " +
			"0 0 30px rgba(255,255,255,0.35)",
	},



};

export default OffsetCard;
