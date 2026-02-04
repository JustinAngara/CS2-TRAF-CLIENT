import "./offsets.css";

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
		return <div className="offsetCard">Invalid block</div>;
	}

	const key = block.Key ?? -1;
	const title = block.Name?.trim() ? block.Name : `Table ${key}`;
	const kind = block.Type?.trim() ? block.Type : "UNKNOWN";

	const list: OffsetItem[] = Array.isArray(block.List) ? block.List : [];
	const items = block.Items ?? list.length;

	return (
		<div className="offsetCard">
			<div className="cardHeader">
				<h3 className="cardTitle">{title}</h3>

				<div className="metaRow">
					<span className={kind === "NAMESPACE" ? "badgeNamespace" : "badgeEnum"}>
						{kind}
					</span>
					<span className="muted">Items: {items}</span>
				</div>
			</div>

			{list.length === 0 ? (
				<div className="muted">No items</div>
			) : (
				<div className="list">
					<div className="offsetRow headerRow">
						<div className="hName">Name</div>
						<div className="hOffset">Offset</div>
						<div className="hType">Type</div>
					</div>

					{list.map((it, idx) => (
						<div key={`${key}-${it.Name ?? "item"}-${idx}`} className="offsetRow">
							<div className="name">{it.Name ?? ""}</div>
							<div className="offset">{it.Offset ?? ""}</div>
							<div className="type">
								{it.Type && it.Type.trim().length > 0 ? it.Type : "uintptr_t"}
							</div>
						</div>
					))}
				</div>
			)}
		</div>
	);
};

export default OffsetCard;
